//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "UberShaderScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "../../Materials/UberMaterial.h"


UberShaderScene::UberShaderScene(void) :
	GameScene(L"ModelTestScene"),
	m_pTeapot(nullptr)
{
}


UberShaderScene::~UberShaderScene(void)
{
}

void UberShaderScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//TEAPOT OBJECT
	//************
	m_pTeapot = new GameObject();

	auto modelComponent = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	auto uberMaterial = new UberMaterial();
	uberMaterial->EnableDiffuseTexture(true);
	uberMaterial->SetDiffuseTexture(L"./Resources/Textures/CobbleStone_DiffuseMap.dds");
	uberMaterial->EnableSpecularLevelTexture(true);
	uberMaterial->SetSpecularLevelTexture(L"./Resources/Textures/CobbleStone_HeightMap.dds");
	uberMaterial->EnableNormalMapping(true);
	uberMaterial->SetNormalMapTexture(L"./Resources/Textures/CobbleStone_NormalMap.dds");
	uberMaterial->EnableEnvironmentMapping(true);
	uberMaterial->SetEnvironmentCube(L"./Resources/Textures/Sunol_Cubemap.dds");
	uberMaterial->EnableSpecularPhong(true);
	gameContext.pMaterialManager->AddMaterial(uberMaterial, 0);
	modelComponent->SetMaterial(0);
	m_pTeapot->AddComponent(modelComponent);

	AddChild(m_pTeapot);

}

void UberShaderScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void UberShaderScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
