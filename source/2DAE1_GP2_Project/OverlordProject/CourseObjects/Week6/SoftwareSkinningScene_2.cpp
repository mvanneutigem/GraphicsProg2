
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SoftwareSkinningScene_2.h"
#include "Physx/PhysxProxy.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/TransformComponent.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2(void) :
	GameScene(L"SoftwareSkinningsCene")
{
	m_pBone0 = nullptr;
	m_pBone1 = nullptr;
	m_BoneRotation = 0;
	m_RotationSign = 1;
}


SoftwareSkinningScene_2::~SoftwareSkinningScene_2(void)
{
}

void SoftwareSkinningScene_2::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto colMat = new ColorMaterial(true);
	gameContext.pMaterialManager->AddMaterial(colMat, 1);

	auto root = new GameObject();
	m_pBone0 = new BoneObject(0, 1, 15);
	m_pBone1 = new BoneObject(1, 1, 15);
	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);
	root->GetTransform()->Rotate(0, -90, 0);
	AddChild(root);
	m_pBone0->CalculateBindPose();
	auto meshes = new GameObject();
	m_pMeshDrawer = new MeshDrawComponent(24);
	meshes->AddComponent(m_pMeshDrawer);
	AddChild(meshes);
	CreateMesh(15);
}

void SoftwareSkinningScene_2::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45)
		m_RotationSign = 1;
	if (m_RotationSign > 0 && m_BoneRotation >= 45)
		m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0, 0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2, 0, 0);

	//part 2
	auto bind0 = m_pBone0->GetBindPose();
	auto bonebind0 = XMLoadFloat4x4(&bind0);
	auto boneworld0 = XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	auto boneTransform0 = XMMatrixMultiply(bonebind0, boneworld0);

	auto bind1 = m_pBone1->GetBindPose();
	auto bonebind1 = XMLoadFloat4x4(&bind1);
	auto boneworld1 = XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	auto boneTransform1 = XMMatrixMultiply(bonebind1, boneworld1);

	for (unsigned int i = 0; i < m_SkinnedVertices.size() ; ++i)
	{
		if ( i < 24)
		{
			XMVECTOR originalVert = XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, XMVector3TransformCoord(originalVert, boneTransform0));
		}
		else
		{
			XMVECTOR originalVert = XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
			XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, XMVector3TransformCoord(originalVert, boneTransform1));
		}
	}

	m_pMeshDrawer->RemoveTriangles();
	for (unsigned int i = 0; i < m_SkinnedVertices.size() / 4; ++i)
	{
		auto quad = QuadPosNormCol(m_SkinnedVertices[i * 4].TransformedVertex,
			m_SkinnedVertices[i * 4 + 1].TransformedVertex,
			m_SkinnedVertices[i * 4 + 2].TransformedVertex,
			m_SkinnedVertices[i * 4 + 3].TransformedVertex);
		
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SoftwareSkinningScene_2::CreateMesh(float length)
{
	auto pos = XMFLOAT3(length / 2, 0, 0); 
	auto offset = XMFLOAT3(length / 2, 2.5f, 2.5f); 
	auto col = XMFLOAT4(1, 0, 0, 0.5f);
	//***** //BOX1* //*****
	//FRONT 
	auto norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
																																																													   //***** //BOX2* //*****
	col = XMFLOAT4(0, 1, 0, 0.5f); pos = XMFLOAT3(22.5f, 0, 0); 
	//FRONT 
	norm = XMFLOAT3(0, 0, -1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BACK 
	norm = XMFLOAT3(0, 0, 1); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//TOP 
	norm = XMFLOAT3(0, 1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	//BOTTOM 
	norm = XMFLOAT3(0, -1, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//LEFT 
	norm = XMFLOAT3(-1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	//RIGHT 
	norm = XMFLOAT3(1, 0, 0); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col)); 
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col)); 
}
