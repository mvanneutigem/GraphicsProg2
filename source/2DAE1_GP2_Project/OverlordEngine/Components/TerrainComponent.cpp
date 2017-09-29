//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainComponent.h"

#include "..\Base\GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "TransformComponent.h"
#include "../Graphics/TextureData.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "Components.h"
#include <limits.h>
#include "../Diagnostics/DebugRenderer.h"
#include "../Scenegraph/SceneManager.h"


TerrainComponent::TerrainComponent(const wstring& heightMapFile, const wstring& textureFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight) :
	m_HeightMapFile(heightMapFile),
	m_TextureFile(textureFile),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows*nrOfColumns),
	m_NrOfTriangles(2 * (nrOfRows - 1) * (nrOfColumns - 1)),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{
	
} 

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	CreatePxHeightField();

}

void TerrainComponent::Update(const GameContext& gameContext)
{ 
	UNREFERENCED_PARAMETER(gameContext);
	//add debugrenderer stuff only once per scenecall #nolag
	if (!DebugRenderer::AddedFixedGeometry() && m_pGameObject->GetScene()->GetPhysxProxy()->IsPhysxDebugRenderingEnabled())
	{
		vector<VertexPosCol> debugArr;
		for (unsigned int i = 0; i < m_NumIndices; ++i)
		{
			if (i%3 != 0)
				debugArr.push_back(VertexPosCol(m_VecVertices[m_VecIndices[i]].Position, XMFLOAT4(Colors::White)));
		}
		if (m_pGameObject->GetScene()->GetPhysxProxy()->IsPhysxDebugRenderingEnabled())
		{
			DebugRenderer::AddFixedGeometry(gameContext.pDevice, debugArr);
		}
		addedDebug = true;
	}
}

void TerrainComponent::Draw(const GameContext& gameContext)
{ 
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p< techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	m_VecHeightValues.resize(m_NrOfVertices, 0);
	std::ifstream inFile;
	inFile.open(m_HeightMapFile.c_str(), std::ios_base::binary);
	if(!inFile)
	{
		Logger::LogFormat(LogLevel::Error, L"Loading terrain \'%ls\' failed!", m_HeightMapFile.c_str());
		return;
	}

	inFile.read(reinterpret_cast<char*>(&m_VecHeightValues[0]), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	inFile.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	//**VERTICES
	//Reserve spots in the buffer
	m_VecVertices.resize(sizeof(VertexPosNormTex)* m_NrOfVertices);

	//Calculate the Initial Position (Terrain centered on the origin)
	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float halfWidth = -m_Width / 2.f;
	float cellXpos = halfWidth;
	float halfDepth = m_Depth / 2.f;
	float cellZpos = halfDepth;
	float cellYpos = 0;

	//Reset the cellXPos Position for each Column
	for(unsigned int r = 0; r < m_NrOfRows; ++r)
	{
		cellXpos = -m_Width / 2.0f;
		for(unsigned int c = 0; c < m_NrOfColumns; ++c)
		{
			int vertexId = r * m_NrOfColumns + c;
			float fraction = (float(m_VecHeightValues[vertexId]) / USHRT_MAX);
			cellYpos = fraction * m_MaxHeight;
			m_VecVertices[vertexId].Position.x = cellXpos;
			m_VecVertices[vertexId].Position.y = cellYpos;
			m_VecVertices[vertexId].Position.z = cellZpos;

			//texcoords
			m_VecVertices[vertexId].TexCoord.x = (cellXpos - halfWidth) / m_Width ;
			m_VecVertices[vertexId].TexCoord.y = (-cellZpos - halfDepth) / m_Depth ;

			cellXpos += cellWidth;
		}
		cellZpos -= cellDepth;
	}


	int nrQuadsRow = m_NrOfRows - 1;
	int nrOfQuadColumn = m_NrOfColumns - 1;

	for(int r = 0; r < nrQuadsRow; ++r)
	{
		for (int col = 0; col < nrOfQuadColumn; ++col)
		{
			int a = r * m_NrOfColumns + col;
			int b = a + 1;
			int c = a + m_NrOfColumns;
			int d = c + 1;
			AddQuad(a, b, c, d);
		}
	}

	for (unsigned int i = 0; i < m_VecIndices.size(); i += 6)
	{
		XMVECTOR a, b, c, d, e, f;
		a = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i)).Position);
		b = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 1)).Position);
		c = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 2)).Position);
		d = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 3)).Position);
		e = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 4)).Position);
		f = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 5)).Position);

		XMVECTOR v1, v2, normal;
		v1 = a - c;
		v2 = a - b;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMFLOAT3 normalFloat3;
		XMStoreFloat3(&normalFloat3, normal);
		m_VecFaceNormals.push_back(normalFloat3);

		v1 = f - e;
		v2 = f - d;
		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
		XMStoreFloat3(&normalFloat3, normal);
		m_VecFaceNormals.push_back(normalFloat3);
	}

	int numFacesPerRow = (m_NrOfColumns - 1) * 2;
	XMFLOAT3 normals[6];
	int index[6];

	for (unsigned int row = 0; row < m_NrOfRows; ++row)
	{
		for (unsigned int col = 0; col < m_NrOfColumns; ++col)
		{
			int centerindex = numFacesPerRow * row + col * 2;
			index[0] = centerindex - 1;
			index[1] = centerindex;
			index[2] = centerindex + 1;
			index[3] = centerindex - numFacesPerRow - 2;
			index[4] = centerindex - numFacesPerRow - 1;
			index[5] = centerindex - numFacesPerRow - 0;

			if (col == 0)
			{
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}
			if (col == m_NrOfRows)
			{
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}
			XMVECTOR sum;
			for (int i = 0; i < 6; ++i)
			{
				if ((index[i] >= 0) && (index[i] < (int)m_VecFaceNormals.size()))
				{
					sum += XMLoadFloat3(&m_VecFaceNormals.at(index[i]));
				}
			}
			int vertexId = row * m_NrOfColumns + col;
			XMVector3Normalize(sum);
			XMFLOAT3 sumFl;
			XMStoreFloat3(&sumFl, sum);
			m_VecFaceNormals[vertexId] = sumFl;
		}
	}

	//Reset the cellXPos Position for each Column
	for (unsigned int r = 0; r < m_NrOfRows; ++r)
	{
		for (unsigned int c = 0; c < m_NrOfColumns; ++c)
		{
			int vertexId = r * m_NrOfColumns + c;
			//normals
			m_VecVertices[vertexId].Normal = m_VecFaceNormals[vertexId];
		}
	}

	//1. Position -- Partially Exercise - Heightmap --
	//2. Normal
	//3. TexCoord -- Exercise - UV --
	
	//Move the cellXPos Position (Down)
	//Move the cellZPos Position (Right)

	//Exercise - Normals
	//For each face...
	//Get the positions of 6 vertices
	//first triangle
	//second triangle

	//iterate through the vertices and calculate a normal for each one of them using the average of the 6 adjacent faces

	//from left front to right back
	//if col==0 is on left edge, there are 
	//no vertices on the left, fill in a illegal index

	//if col==m_NumColumns-1 is on right edge, there are 
	//no vertices on the right, fill in a illegal index

	//if index<0 or out of range: front or back edge 
	//it may not be used to calculate the average

	//calculate average by normalizing
}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d)
{
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

//Exercise - PhysX
void TerrainComponent::CreatePxHeightField()
{
	//Implement PhysX HeightField
	PxHeightFieldSample* samples = new PxHeightFieldSample[m_VecVertices.size()];
	
	for (unsigned int i = 0; i < m_VecVertices.size(); ++i)
	{
		samples[i].height = PxI16( m_VecVertices[i].Position.y);
	}

	PxHeightFieldDesc hfDesc;
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = m_NrOfColumns;
	hfDesc.nbRows = m_NrOfRows;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	PxHeightField* aHeightField = physX->createHeightField(hfDesc);

	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float  heightScale = 1;
	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), heightScale, cellWidth,
		cellDepth);

	auto terrainMaterial = physX->createMaterial(0.5, 0.5, 1);
	PxRigidStatic* hfActor = physX->createRigidStatic(PxTransform(PxVec3(-m_Width/2.f,0,m_Depth/2.0f), PxQuat(0.5*XM_PI, PxVec3(0,1,0))));
	hfActor->setActorFlag(PxActorFlag::eVISUALIZATION, false);
	PxShape* hfShape = hfActor->createShape(hfGeom, *terrainMaterial);
	//set collision groups!
	auto filter_data = PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0);
	hfShape->setSimulationFilterData(filter_data);
	hfShape->setQueryFilterData(filter_data);
	hfShape->setFlag(PxShapeFlag::eVISUALIZATION, false);//prevent the debugrenderer from adding the data every frame
	m_pGameObject->GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*hfActor);
	if (samples)
		delete[] samples;

	//After implementing the physX representation - Fix the lagginess from the PhysX Debugger!!!

	//the lagginess is caused by appending all this data every single frame even tho it's static but what do you want me to do about it? rewrite the debugrenderer???
	//well then i guess so: changed the debug rendere, gamescene and physxproxy classes so if it fucks up later i know what to change back
}
