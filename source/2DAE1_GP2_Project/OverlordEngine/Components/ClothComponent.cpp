#include "stdafx.h"
#include "ClothComponent.h"
#include "Components.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Content/ContentManager.h"

ClothComponent::ClothComponent(const PxTransform &pose, int resolution, int sizeX, int sizeY, const wstring& textureFile):
m_pose(pose),
m_Resolution(resolution),
m_SizeX(sizeX),
m_SizeY(sizeY),
m_TextureFile(textureFile)
{

}

ClothComponent::~ClothComponent()
{
	m_pCloth->release();

	delete[] triangles;
	
}



void ClothComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// create regular mesh
	PxU32 numParticles = m_Resolution*m_Resolution;
	m_numTriangles = 2 * (m_Resolution - 1)*(m_Resolution - 1);

	// create cloth particles
	particles = new PxClothParticle[numParticles];
	PxVec3 center(PxReal(m_SizeX/2.0f), 0, PxReal(-m_SizeY));
	PxVec3 delta = 1.0f / (m_Resolution - 1) * PxVec3(PxReal(m_SizeX), 0.f, PxReal( m_SizeY));
	PxClothParticle* pIt = particles;
	for (PxU32 i = 0; i<PxU32(m_Resolution); ++i)
	{
		for (PxU32 j = 0; j<PxU32(m_Resolution); ++j, ++pIt)
		{
			pIt->invWeight = j + 1<PxU32(m_Resolution) ? 1.0f : 0.0f;
			pIt->pos = delta.multiply(PxVec3(PxReal(i),
				PxReal(j), -PxReal(j))) - center;
		}
	}

	// create triangles
	triangles = new PxU32[3 * m_numTriangles];
	PxU32* iIt = triangles;
	for (PxU32 i = 0; i<PxU32(m_Resolution) - 1; ++i)
	{
		for (PxU32 j = 0; j<PxU32(m_Resolution) - 1; ++j)
		{
			PxU32 odd = j & 1u, even = 1 - odd;
			*iIt++ = i*m_Resolution + (j + odd);
			*iIt++ = (i + odd)*m_Resolution + (j + 1);
			*iIt++ = (i + 1)*m_Resolution + (j + even);
			*iIt++ = (i + 1)*m_Resolution + (j + even);
			*iIt++ = (i + even)*m_Resolution + j;
			*iIt++ = i*m_Resolution + (j + odd);
		}
	}

	// create fabric from mesh
	PxClothMeshDesc meshDesc;
	meshDesc.points.count = numParticles;
	meshDesc.points.stride = sizeof(PxClothParticle);
	meshDesc.points.data = particles;

	meshDesc.invMasses.count = numParticles;
	meshDesc.invMasses.stride = sizeof(PxClothParticle);
	meshDesc.invMasses.data = &particles->invWeight;

	meshDesc.triangles.count = m_numTriangles;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = triangles;

	// cook fabric
	PxClothFabric* fabric = PxClothFabricCreate(*PhysxManager::GetInstance()->GetPhysics(), meshDesc, PxVec3(0, 1, 0));

	// create cloth
	PxTransform gPose = m_pose;
	m_pCloth = PhysxManager::GetInstance()->GetPhysics()->createCloth(gPose, *fabric, particles, PxClothFlags(0));

	fabric->release();
	delete[] particles;

	m_pCloth->setSolverFrequency(240.0f);

	//collision made for char
	PxTransform invPose = gPose.getInverse();
	PxClothCollisionSphere spheres[3];
	for (int i = 0; i<3; ++i)
	{
		spheres[i].pos = invPose.transform(gSpheres[i].pos);
		spheres[i].radius = gSpheres[i].radius;
	}
	m_pCloth->setCollisionSpheres(spheres, 3);
	m_pCloth->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);
	m_pCloth->setClothFlag(PxClothFlag::eSWEPT_CONTACT, true);
	
	auto physxscene = m_pGameObject->GetScene()->GetPhysxProxy()->GetPhysxScene();
	physxscene->addActor(*m_pCloth);

	//drawing the cloth
	m_pMeshDrawer = GetTransform()->GetGameObject()->GetComponent<MeshDrawComponent>();

	if (!m_pMeshDrawer)
		Logger::LogError(L"Cannot have a cloth component without a meshdrawcomponent ");

}

void ClothComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//update particles pos etc
	particles = m_pCloth->lockParticleData()->particles;

	m_pMeshDrawer->RemoveTriangles();
	if (m_pMeshDrawer)
	{
		m_pMeshDrawer->SetTriangleCapacity(m_numTriangles);
		for (unsigned int i = 0; i < m_numTriangles*3; i +=3)
		{
			m_pMeshDrawer->AddTriangle(
				VertexPosNormCol(ToXMFLOAT3(particles[triangles[i]].pos), { 0,0,1 }, XMFLOAT4(Colors::Red)),
				VertexPosNormCol(ToXMFLOAT3(particles[triangles[i + 1]].pos), { 0,0,1 }, XMFLOAT4(Colors::Red)),
				VertexPosNormCol(ToXMFLOAT3(particles[triangles[i + 2]].pos), { 0,0,1 }, XMFLOAT4(Colors::Red))
			);
		}
	}
	m_pMeshDrawer->UpdateBuffer();
}

void ClothComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ClothComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}
void ClothComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d)
{
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}