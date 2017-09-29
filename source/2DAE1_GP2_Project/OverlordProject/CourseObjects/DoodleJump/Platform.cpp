#include "stdafx.h"
#include "Platform.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow_Outline.h"
#include "Components/ModelComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Content/ContentManager.h"
#include "Components/ColliderComponent.h"
#include "Components/TransformComponent.h"
#include "Physx/PhysxManager.h"
#include "JumpCharacter.h"
#include "../../Materials/shader/CrumbleMaterial.h"

//just change their position when out of frame

Platform::Platform(float x, float y, float size, bool breakable, int ID ):
	m_Position(x,y, 0),
	m_Scale(size,size,size),
	m_Breakable(breakable),
	m_BreakableID(20+ID)
{
}


Platform::~Platform()
{
}

void Platform::Initialize(const GameContext& gameContext)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	//mat
	if(m_Breakable)
	{

		auto diffMat3 = new CrumbleMaterial();
		diffMat3->SetDiffuseTexture(L"./Resources/Textures/platform_1_red.jpg");
		diffMat3->SetFallSeedX(9);
		diffMat3->SetFallSeedY(70);
		diffMat3->SetFallMultiplier(20);
		diffMat3->SetThickness(1);
		diffMat3->SetGridSize(10);
		diffMat3->SetBlockSize(3);
		diffMat3->PauseTime(true);
		diffMat3->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

		gameContext.pMaterialManager->AddMaterial(diffMat3, m_BreakableID);

	}
	
	//mesh
	platform = new GameObject();
	auto platformModel = new ModelComponent(L"./Resources/Meshes/platform_Rot.ovm");
	if(m_Breakable)
		platformModel->SetMaterial(m_BreakableID);
	else
		platformModel->SetMaterial(2);

	auto pRigidBody = new RigidBodyComponent();
	platform->AddComponent(pRigidBody);

	auto mesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/platform_Rot.ovpc");
	PxMeshScale scale(PxVec3(0.35f * m_Scale.x, 0.35f* m_Scale.x, 0.35f* m_Scale.x), PxQuat(1));
	std::shared_ptr<PxGeometry> meshgeom(new PxConvexMeshGeometry(mesh, scale));

	platform->AddComponent(new ColliderComponent(meshgeom, *bouncyMaterial));
	platform->GetComponent<ColliderComponent>()->EnableTrigger(true);

	platform->GetTransform()->Translate(m_Position.x, m_Position.y,m_Position.z);
	platform->AddComponent(platformModel);
	AddChild(platform);
}

void Platform::PostInitialize(const GameContext& gameContext)
{
	//move
	UNREFERENCED_PARAMETER(gameContext);
	float random = float( rand() % 270);
	platform->GetTransform()->Scale(0.35f* m_Scale.x, 0.35f* m_Scale.x, 0.35f* m_Scale.x);
	platform->GetTransform()->Rotate(0, random, 0);
	platform->GetComponent<RigidBodyComponent>()->SetKinematic(true);
}

void Platform::Update(const GameContext& gameContext)
{
	if(m_Reset)
	{
		//reset mat
		if (m_Breakable)
		{
			m_Broken = false;
			auto mat = gameContext.pMaterialManager->GetMaterial(m_BreakableID);
			auto cmat = dynamic_cast<CrumbleMaterial*> (mat);
			cmat->PauseTime(true);
			cmat->SetTime(0.f);
		}
		m_Reset = false;
	}

	platform->SetOnTriggerCallBack([this, gameContext](GameObject* trigger, GameObject* other, GameObject::TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		JumpCharacter* ch = dynamic_cast<JumpCharacter*> (other);
		if (ch != NULL && action == GameObject::TriggerAction::ENTER && !m_Broken)
		{		
			if(ch->GetTransform()->GetPosition().y > GetTransform()->GetPosition().y - 10)
			{
				//character fall on platform
				bool succes = false;
				ch->Jump(succes);
				if(succes && m_Breakable)
				{
					auto mat = gameContext.pMaterialManager->GetMaterial(m_BreakableID);
					dynamic_cast<CrumbleMaterial*> (mat)->PauseTime(false);
					m_Broken = true;
				}
			}
		}
	});
}
