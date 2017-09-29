#include "stdafx.h"
#include "Enemy.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ModelComponent.h"
#include "Content/ContentManager.h"
#include "Components/ColliderComponent.h"
#include "Physx/PhysxManager.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow_Outline.h"
#include "JumpCharacter.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"


Enemy::Enemy(wstring model, wstring collider, int matID, XMFLOAT3 scale):
PickUp(model, collider, matID, scale)
{
}


Enemy::~Enemy()
{
}

void Enemy::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//anim

	if(!m_SetAnim)
	{
		auto anim = m_GameObject->GetComponent<ModelComponent>()->GetAnimator();
		anim->SetAnimation(0);
		anim->Play();

		m_GameObject->SetTag(L"Enemy");
		m_GameObject->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
		m_SetAnim = true;
	}

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
