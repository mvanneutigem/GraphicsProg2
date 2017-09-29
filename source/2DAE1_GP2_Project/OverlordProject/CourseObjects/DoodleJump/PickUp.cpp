#include "stdafx.h"
#include "PickUp.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ModelComponent.h"
#include "Content/ContentManager.h"
#include "Components/ColliderComponent.h"
#include "Physx/PhysxManager.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow_Outline.h"
#include "JumpCharacter.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"


PickUp::PickUp(wstring model, wstring collider, int matID, XMFLOAT3 scale):
m_Model(model),
m_Collider(collider),
m_ID(matID),
m_Scale(scale)
{
}


PickUp::~PickUp()
{
}

void PickUp::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	m_GameObject = new GameObject();
	auto platformModel = new ModelComponent(m_Model);
	platformModel->SetMaterial(m_ID);
	m_GameObject->AddComponent(platformModel);

	auto pRigidBody = new RigidBodyComponent();
	m_GameObject->AddComponent(pRigidBody);

	auto mesh = ContentManager::Load<PxConvexMesh>(m_Collider);
	PxMeshScale scale(PxVec3(m_Scale.x, m_Scale.x,m_Scale.x), PxQuat(1));
	std::shared_ptr<PxGeometry> meshgeom(new PxConvexMeshGeometry(mesh, scale));

	m_GameObject->AddComponent(new ColliderComponent(meshgeom, *bouncyMaterial));
	m_GameObject->GetComponent<ColliderComponent>()->EnableTrigger(true);

	AddChild(m_GameObject);
}

void PickUp::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//m_Triggered = false;
	m_GameObject->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		JumpCharacter* ch = dynamic_cast<JumpCharacter*> (other);
		if (ch != NULL && action == GameObject::TriggerAction::ENTER)
		{
			m_Triggered = true;
		}
	});
}

void PickUp::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_GameObject->GetTransform()->Translate(0, -20, 0);
	m_GameObject->GetTransform()->Rotate(-90, 0, 0);
	m_GameObject->GetComponent<RigidBodyComponent>()->SetKinematic(true);
}

bool PickUp::TriggeredByCharacter() 
{
	if(m_Triggered)
	{
		m_Triggered = false;
		return true;
	}
	return false;
}
