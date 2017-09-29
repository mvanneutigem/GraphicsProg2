#include "stdafx.h"
#include "CharacterTest.h"

#include <Prefabs/TorusPrefab.h>
#include <Components/Components.h>
#include <Physx/PhysxProxy.h>
#include "Physx/PhysxManager.h"
#include "Character.h"


CharacterTest::CharacterTest() :
	GameScene(L"CharacterTest")
{
}


CharacterTest::~CharacterTest()
{
}

void CharacterTest::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//PhysX
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	//GROUND
	//****************
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//character
	m_pChar = new Character();
	m_pChar->GetTransform()->Translate(0, 10, 0);
	AddChild((m_pChar));
}

void CharacterTest::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTest::SceneActivated()
{
}

void CharacterTest::SceneDeactivated()
{
}
