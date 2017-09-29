#include "stdafx.h"
#include "Pong.h"
#include <Prefabs/Prefabs.h>
#include <Components/Components.h>
#include <PhysX/PhysxProxy.h>
#include "PhysX/PhysxManager.h"
#include "Scenegraph\GameObject.h"
#include "Base/InputManager.h"
#include "Components/Components.h"

//typedef std::function<void(GameObject* triggerObject, GameObject* other, TriggerAction action)> PhysicsCallback;

Pong::Pong() : GameScene(L"PongScene")
{
}


Pong::~Pong()
{
}

void Pong::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//GetPhysxProxy()->EnablePhysXFrameStepping(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	//Physx
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto notThatBouncyMaterial = physX->createMaterial(0.01f, 0.01f, 0.5f);
	auto bouncyMaterial = physX->createMaterial(0.01f, 0.01f, 1.1f);

	//GROUND PLANE
	auto pGround = new CubePrefab(100.f,100.f,100.0f,XMFLOAT4(Colors::Black));
	pGround->AddComponent(new RigidBodyComponent(true));
	pGround->GetTransform()->Translate(0, -50.f, 0);
	std::shared_ptr<PxGeometry> geom(new PxBoxGeometry(50.f,50.f,50.f));
	pGround->AddComponent(
		new ColliderComponent(
			geom,
			*notThatBouncyMaterial,
			PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1))))
		);

	AddChild(pGround);

	//cubeborder
	auto pCube2 = new CubePrefab(50.f,1.0f,1.0f, XMFLOAT4(Colors::White));
	pCube2->GetTransform()->Translate(0.f, 0.51f, -12.f);

	auto pRigid2 = new RigidBodyComponent();
	pRigid2->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | TransZ | RotX | RotY | RotZ), true);
	pCube2->AddComponent(pRigid2);

	std::shared_ptr<PxGeometry> cubeGeom(new PxBoxGeometry(25.f,0.5f,0.5f));
	pCube2->AddComponent(new ColliderComponent(cubeGeom, *bouncyMaterial));
	AddChild(pCube2);

	//cubeborder2
	auto pCube = new CubePrefab(50.f, 1.0f, 1.0f, XMFLOAT4(Colors::White));
	pCube->GetTransform()->Translate(0.f, 0.51f, 12.f);

	auto pRigid = new RigidBodyComponent();
	pRigid->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | TransZ | RotX | RotY | RotZ), true);
	pCube->AddComponent(pRigid);

	pCube->AddComponent(new ColliderComponent(cubeGeom, *bouncyMaterial));
	AddChild(pCube);

	//playerLeft
	m_pBatL = new CubePrefab(1.f, 1.0f, 6.0f, XMFLOAT4(Colors::White));
	m_pBatL->GetTransform()->Translate(-20.f, 0.51f, 0.f);

	auto pRigid3 = new RigidBodyComponent();
	pRigid3->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | RotX | RotY | RotZ), true);
	pRigid3->SetKinematic(true);
	m_pBatL->AddComponent(pRigid3);

	std::shared_ptr<PxGeometry> batGeom(new PxBoxGeometry(0.5f, .5f, 3.f));
	m_pBatL->AddComponent(new ColliderComponent(batGeom, *bouncyMaterial));
	AddChild(m_pBatL);

	//playerright
	m_pBatR = new CubePrefab(1.f, 1.0f, 6.0f, XMFLOAT4(Colors::White));
	m_pBatR->GetTransform()->Translate(20.f, 0.51f, 0.f);

	auto pRigid4 = new RigidBodyComponent();
	pRigid4->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | RotX | RotY | RotZ), true);
	pRigid4->SetKinematic(true);
	m_pBatR->AddComponent(pRigid4);

	m_pBatR->AddComponent(new ColliderComponent(batGeom, *bouncyMaterial));
	AddChild(m_pBatR);

	//ball
	m_pBall = new SpherePrefab(1.0f, 10, XMFLOAT4(Colors::White));
	m_pBall->GetTransform()->Translate(0, 0.5f, 0);

	auto pRigid5 = new RigidBodyComponent();
	//pRigid5->SetConstraint(RigidBodyConstraintFlag(TransY), true);
	m_pBall->AddComponent(pRigid5);

	std::shared_ptr<PxGeometry> sphereGeom(new PxSphereGeometry(1.0f));
	m_pBall->AddComponent(new ColliderComponent(sphereGeom, *bouncyMaterial));

	AddChild(m_pBall);

	//camera
	auto fixedCamera = new FixedCamera();
	AddChild(fixedCamera);
	fixedCamera->GetComponent<TransformComponent>()->Translate(0, 30, 0);
	fixedCamera->GetComponent<TransformComponent>()->Rotate(90, 0, 0);
	fixedCamera->GetComponent<CameraComponent>()->UseOrthographicProjection();
	gameContext.pCamera->SetActive();
	auto gameScene = fixedCamera->GetScene();
	gameScene->SetActiveCamera(fixedCamera->GetComponent<CameraComponent>());

	//input
	gameContext.pInput->AddInputAction(InputAction(0, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(1, Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(2, Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(3, Down, 'S'));

	//triggers
	//right trigger
	m_pTriggerActorRight = new GameObject();
	m_pTriggerActorRight->GetTransform()->Translate(-25.f, 0.51f, 0.f);

	auto pRigid6 = new RigidBodyComponent();
	pRigid6->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | TransZ | RotX | RotY | RotZ), true);
	
	m_pTriggerActorRight->AddComponent(pRigid6);

	std::shared_ptr<PxGeometry> triggerGeom(new PxBoxGeometry(.5f, 0.5f, 15.f));
	m_pTriggerActorRight->AddComponent(new ColliderComponent(triggerGeom, *notThatBouncyMaterial));
	m_pTriggerActorRight->GetComponent<ColliderComponent>()->EnableTrigger(true);
	AddChild(m_pTriggerActorRight);

	//left trigger
	m_pTriggerActorLeft = new GameObject();
	m_pTriggerActorLeft->GetTransform()->Translate(25.f, 0.51f, 0.f);

	auto pRigid7 = new RigidBodyComponent();
	pRigid7->SetConstraint(RigidBodyConstraintFlag(TransY | TransX | TransZ | RotX | RotY | RotZ), true);

	m_pTriggerActorLeft->AddComponent(pRigid7);

	m_pTriggerActorLeft->AddComponent(new ColliderComponent(triggerGeom, *notThatBouncyMaterial));
	m_pTriggerActorLeft->GetComponent<ColliderComponent>()->EnableTrigger(true);
	AddChild(m_pTriggerActorLeft);
	
}

void Pong::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_StartGame)
	{
		PxVec3 Force;
		Force.x = PxReal(rand() % 100);
		Force.y = 0;
		Force.z = PxReal(rand() % 50);

		auto det = rand() % 100;
		if (det > 50)
		{
			Force *= -1;
		}

		Force.normalize();
		Force *= 10.0f;
		m_pBall->GetTransform()->Translate(0, 0.5f, 0);
		m_pBall->GetComponent<RigidBodyComponent>()->AddForce(Force, PxForceMode::eIMPULSE);
		m_StartGame = false;
	}

	if (gameContext.pInput->IsActionTriggered(2))
	{
		m_LeftPos = m_pBatL->GetTransform()->GetPosition().z;
		if (m_LeftPos <= 8.4f)
		{
			m_LeftPos += 10.0f * gameContext.pGameTime->GetElapsed();
			m_pBatL->GetTransform()->Translate(-20, 0.51f, m_LeftPos);
		}
	}
	else if (gameContext.pInput->IsActionTriggered(3))
	{
		m_LeftPos = m_pBatL->GetTransform()->GetPosition().z;
		if (m_LeftPos >= -8.4f)
		{
			m_LeftPos -= 10.0f * gameContext.pGameTime->GetElapsed();
			m_pBatL->GetTransform()->Translate(-20, 0.51f, m_LeftPos);
		}
	}
	if (gameContext.pInput->IsActionTriggered(0))
	{
		m_RightPos = m_pBatR->GetTransform()->GetPosition().z;
		if (m_RightPos <= 8.4f)
		{
			m_RightPos += 10.0f * gameContext.pGameTime->GetElapsed();
			m_pBatR->GetTransform()->Translate(20, 0.51f, m_RightPos);
		}
	}
	else if (gameContext.pInput->IsActionTriggered(1))
	{
		m_RightPos = m_pBatR->GetTransform()->GetPosition().z;
		if (m_RightPos >= -8.4f)
		{
			m_RightPos -= 10.0f * gameContext.pGameTime->GetElapsed();
			m_pBatR->GetTransform()->Translate(20, 0.51f, m_RightPos);
		}
	}

	m_pTriggerActorRight->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		if (other == m_pBall && action == GameObject::TriggerAction::ENTER)
		{
			++m_LeftScore;
			cout << "Right score : " + std::to_string(m_RightScore) + ", left score: " + std::to_string(m_LeftScore) << endl;
			m_StartGame = true;
		}
	});

	m_pTriggerActorLeft->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		if (other == m_pBall && action == GameObject::TriggerAction::ENTER)
		{
				++m_RightScore;
				cout << "Right score : " + std::to_string(m_RightScore) + ", left score: " + std::to_string(m_LeftScore) << endl;
				m_StartGame = true;
		}
	});

}

void Pong::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
void Pong::SceneActivated()
{
	PxVec3 Force;
	Force.x = PxReal(rand() % 100);
	Force.y = 0;
	Force.z = PxReal(rand() % 50);

	auto det = rand() % 100;
	if (det > 50)
	{
		Force *= -1;
	}

	Force.normalize();
	Force *= 10.0f;
	m_pBall->GetComponent<RigidBodyComponent>()->AddForce(Force, PxForceMode::eIMPULSE);
}
void Pong::SceneDeactivated()
{

}