//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Prefabs/SkyBoxPrefab.h"
#include "Components/TerrainComponent.h"

#define FPS_COUNTER 1

TerrainTestScene::TerrainTestScene(void) :
	GameScene(L"TerrainTestScene")
{
}


TerrainTestScene::~TerrainTestScene(void)
{
}

void TerrainTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);

	//ball
	auto pBall = new SpherePrefab();
	auto rb = new RigidBodyComponent();
	pBall->AddComponent(rb);
	std::shared_ptr<PxGeometry> geom(new PxSphereGeometry(1));
	pBall->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform::createIdentity()));
	pBall->GetTransform()->Translate(-10, 20, 0);
	AddChild(pBall);

	//terrain
	auto terrain = new GameObject();
	auto terrainComp = new TerrainComponent(L"Resources/Terrain/Hawai_Heightmap_64x64x16.raw", L"Resources/Terrain/Hawai_TexMap.dds", 64, 64, 100, 100, 20);
	terrain->AddComponent(terrainComp);
	AddChild(terrain);
}

void TerrainTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
