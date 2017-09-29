
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkyBoxScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Prefabs/SkyBoxPrefab.h"

#define FPS_COUNTER 1

SkyBoxScene::SkyBoxScene(void) :
	GameScene(L"SpikeyScene")
{
}


SkyBoxScene::~SkyBoxScene(void)
{
}

void SkyBoxScene::Initialize(const GameContext& gameContext)
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

	//
	auto skybox = new SkyBoxPrefab(L"Resources/Textures/SkyBox.dds");
	AddChild(skybox);
}

void SkyBoxScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyBoxScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
