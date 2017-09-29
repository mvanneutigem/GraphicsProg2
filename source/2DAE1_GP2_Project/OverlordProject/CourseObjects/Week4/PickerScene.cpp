//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PickerScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Graphics\MeshFilter.h"
#include "Components\ModelComponent.h"
#include "../../Materials/UberMaterial.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"

#define FPS_COUNTER 1

PickerScene::PickerScene(void) :
	GameScene(L"SpikeyScene")
{
}


PickerScene::~PickerScene(void)
{
}

void PickerScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	m_pGround = new GameObject();
	m_pGround->AddComponent(new RigidBodyComponent(true));
	m_pGround->SetTag(L"ground");

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	m_pGround->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(m_pGround);

	//TEAPOT OBJECT
	//************
	m_pChair = new GameObject();

	auto modelComponent = new ModelComponent(L"./Resources/Meshes/Chair.ovm");
	auto diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	modelComponent->SetMaterial(1);
	m_pChair->AddComponent(modelComponent);

	auto pRigidBody = new RigidBodyComponent();
	m_pChair->AddComponent(pRigidBody);

	auto mesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Chair.ovpc");
	std::shared_ptr<PxGeometry> chairgeom(new PxConvexMeshGeometry(mesh));
	m_pChair->AddComponent(new ColliderComponent(chairgeom, *bouncyMaterial));

	m_pChair->GetTransform()->Translate(0, 5.f, 0);
	m_pChair->SetTag(L"chair");

	AddChild(m_pChair);

	gameContext.pInput->AddInputAction(InputAction(0, Pressed, -1 ,VK_LBUTTON));
}

void PickerScene::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(0))
	{
		Logger::LogFormat(LogLevel::Info, L"Clicked");
		auto hitObject = gameContext.pCamera->Pick(gameContext);
		if (hitObject != nullptr)
		{
			wstring info = L"Picked " + hitObject->GetTag();
			Logger::LogFormat(LogLevel::Info, info.c_str());
		}
	}
	
}

void PickerScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
