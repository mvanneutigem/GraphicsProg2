//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ClothScene.h"
#include <Scenegraph/GameObject.h>

#include <Content/ContentManager.h>
#include "Components/TransformComponent.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"
#include "Physx/PhysxProxy.h"
#include "Components/ClothComponent.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Prefabs/CubePrefab.h"
#include "Components/ColliderComponent.h"
#include "Physx/PhysxManager.h"
#include "Components/RigidBodyComponent.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"

ClothScene::ClothScene(void) :
	GameScene(L"ClothScene")
{
}


ClothScene::~ClothScene(void)
{
}

void ClothScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	cout << "clothscene";
	auto mat = new DiffuseMaterial();
	mat->Initialize(gameContext);
	mat->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(mat, 1);

	auto cloth = new GameObject();
	auto meshDraw = new MeshDrawComponent(722);
	cloth->AddComponent(meshDraw);
	//always add meshdraw for cloth!
	auto clothComponent = new ClothComponent(PxTransform::createIdentity(),10, 5,10, L"Resources/Terrain/Hawai_TexMap.dds");
	cloth->AddComponent(clothComponent);
	AddChild(cloth);

	auto diffMat3 = new DiffuseMaterial_Shadow();
	diffMat3->SetDiffuseTexture(L"./Resources/Textures/rocket.jpg");
	diffMat3->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat3, 0);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);
	auto obj = new GameObject();
	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetKinematic(true);
	obj->AddComponent(pRigidBody);
	auto model = new ModelComponent(L"./Resources/Meshes/RocketPickup.ovm");
	model->SetMaterial(0);
	auto mesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/RocketPickup.ovpc");
	std::shared_ptr<PxGeometry> meshgeom(new PxConvexMeshGeometry(mesh));
	auto coll = new ColliderComponent(meshgeom, *bouncyMaterial);
	obj->AddComponent(coll);
	obj->AddComponent(model);
	obj->GetTransform()->Translate(0, -5, 3);
	AddChild(obj);
}

void ClothScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ClothScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
