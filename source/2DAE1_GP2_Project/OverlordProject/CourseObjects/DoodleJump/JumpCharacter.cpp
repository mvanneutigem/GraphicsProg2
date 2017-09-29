//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "JumpCharacter.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"
#include "Components/ClothComponent.h"
#include "Components/ParticleEmitterComponent.h"

JumpCharacter::JumpCharacter(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(1.0f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}


JumpCharacter::~JumpCharacter(void)
{
}

void JumpCharacter::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//PhysX
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	// Create controller
	m_pController = new ControllerComponent(bouncyMaterial);
	m_pController->SetCollisionGroup(CollisionGroupFlag::Group1);
	AddComponent(m_pController);

	//cape
	m_cloth = new GameObject();
	auto meshDraw = new MeshDrawComponent(722);//NEVER leave this empty
	m_cloth->AddComponent(meshDraw);
	//always add meshdraw for cloth!
	auto clothComponent = new ClothComponent(PxTransform::createIdentity(), 10, 2, 5, L"Resources/Terrain/Hawai_TexMap.dds");
	m_cloth->AddComponent(clothComponent);

	AddChild(m_cloth);

	// Register all Input Actions
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
	};

	gameContext.pInput->AddInputAction(InputAction(LEFT, Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(RIGHT, Down, 'D'));

	//launch particles
	auto pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 60);
	pParticleEmitter->SetVelocity(XMFLOAT3(0, 1.0f, 0));
	pParticleEmitter->SetMinSize(1.0f);
	pParticleEmitter->SetMaxSize(2.0f);
	pParticleEmitter->SetMinEnergy(1.0f);
	pParticleEmitter->SetMaxEnergy(2.0f);
	pParticleEmitter->SetMinSizeGrow(3.5f);
	pParticleEmitter->SetMaxSizeGrow(6.5f);
	pParticleEmitter->SetMinEmitterRange(0.2f);
	pParticleEmitter->SetMaxEmitterRange(0.5f);
	pParticleEmitter->SetColor(XMFLOAT4(1.f, 1.f, 1.f, 1.0f));
	m_pParticleEmitterObject->AddComponent(pParticleEmitter);
	m_pParticleEmitterObject->GetTransform()->Translate(0, -100, 0);
	AddChild(m_pParticleEmitterObject);
}

void JumpCharacter::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}


void JumpCharacter::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if(m_Velocity.y > 5)
	{
		m_cloth->GetComponent<ClothComponent>()->SetWindVec(PxVec3(0, -30, 10));
	}
	else 
	{
		m_cloth->GetComponent<ClothComponent>()->SetWindVec(PxVec3(0, 30, -10));
	}

	auto dTime = gameContext.pGameTime->GetElapsed();
	dTime *= m_TimeScale;
	//HANDLE INPUT
	XMFLOAT3 move = XMFLOAT3(0, 0, 0);

	move.x = gameContext.pInput->IsActionTriggered(RIGHT) ? 1.0f : 0.0f;
	if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(LEFT) ? 1.0f : 0.0f);

	if (move.x > 0.1 && m_Rotation > -90)
	{
		m_Rotation -= 700 * dTime;
	}
	else if (move.x < -0.1 && m_Rotation < 90)
	{
		m_Rotation += 700 * dTime;
	}
	this->GetChild<GameObject>()->GetTransform()->Rotate(XMFLOAT3(0, m_Rotation, 0));
	m_cloth->GetTransform()->Rotate(XMFLOAT3(0, m_Rotation, 0));

	//CALCULATE TRANSFORMS
	XMFLOAT3 forward = GetTransform()->GetForward();
	forward.y = 0;
	//normalize forwardvector
	float forwardLength = sqrt(forward.x * forward.x + forward.z * forward.z);
	forward.x /= forwardLength;
	forward.z /= forwardLength;

	XMFLOAT3 right = GetTransform()->GetRight();

	auto moveZ = XMFLOAT3(move.z * forward.x, move.z *forward.y, move.z * forward.z);
	auto moveX = XMFLOAT3(move.x * right.x, move.x *right.y, move.x * right.z);
	move = XMFLOAT3(moveZ.x + moveX.x, moveZ.y + moveX.y, moveZ.z + moveX.z);
	auto direction = move;

	//xz movement
	if (move.x != 0 || move.z != 0)
	{
		m_RunAccelerationTime += dTime;
		m_RunVelocity += m_RunAcceleration * m_RunAccelerationTime;
		if (m_RunVelocity > m_MaxRunVelocity)
			m_RunVelocity = m_MaxRunVelocity;

		//normalize velocity
		float m_VelocityLength = sqrt(m_Velocity.x * m_Velocity.x + m_Velocity.z * m_Velocity.z);
		m_Velocity.x /= m_VelocityLength;
		m_Velocity.z /= m_VelocityLength;

		m_Velocity.x = (m_RunVelocity * direction.x);
		m_Velocity.z = (m_RunVelocity * direction.z);
	}
	else
	{
		m_Velocity.x = 0;
		m_Velocity.z = 0;
		m_RunAccelerationTime = 0;
	}

	//y movement
	auto IsOnGround = m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN;
	if (IsOnGround != PxControllerCollisionFlag::eCOLLISION_DOWN && !m_Launching)
	{
		m_JumpVelocity -= m_JumpAcceleration * dTime;
		if (m_JumpVelocity > m_TerminalVelocity)
			m_JumpVelocity = m_TerminalVelocity;
		m_JumpVelocity -= m_Gravity * dTime;

	}
	else if (IsOnGround && !m_Launching)
	{
		m_JumpVelocity = 0;
		m_Velocity.y = 40;

	}
	else if(!m_Launching)
	{
		m_Velocity.y = 0;
	}
	if (!m_Launching)
		m_Velocity.y += m_JumpVelocity*100 * dTime;

	//move
	auto movement = XMFLOAT3{ m_Velocity.x * dTime,  m_Velocity.y * dTime , m_Velocity.z * dTime };
	auto pos = m_pController->GetPosition();
	if (pos.x > 20)
	{
		pos.x = -20;
	}
	if (pos.x < -20)
	{
		pos.x = 20;
	}
	pos.z = 0;
	m_pController->Translate(pos);
	movement.x *= m_MoveSpeed;
	movement.z *= m_MoveSpeed;
	m_pController->Move(movement);

	//jump
	if (m_Jumping)
	{

		m_JumpTime += dTime;
		if (m_JumpTime > 0.45f)
		{
			auto mod = this->GetChild<GameObject>()->GetComponent<ModelComponent>();
			mod->GetAnimator()->SetAnimation(1);
			m_Jumping = false;
			m_JumpTime = 0;
		}
	}

	//launch
	if (m_LaunchTime > 0)
	{
		auto mod = this->GetChild<GameObject>()->GetComponent<ModelComponent>();
		m_Jumping = false;
		m_LaunchTime -= dTime;
		if (m_LaunchTime <= 0)
		{
			mod->GetAnimator()->Pause();
			mod->GetAnimator()->SetAnimation(1);
			mod->GetAnimator()->Play();
			m_pParticleEmitterObject->GetTransform()->Translate(0, -100, 0);
			m_Launching = false;
			m_LaunchTime = 0;
			m_Velocity.y = 0;
		}

		//anim
		if(m_LaunchTime > 4.77f)
		{
			mod->GetAnimator()->Pause();
			mod->GetAnimator()->SetAnimation(2);
			mod->GetAnimator()->Play();
		}
		else if(m_LaunchTime > 0.25f)
		{
			mod->GetAnimator()->Pause();
			mod->GetAnimator()->SetAnimation(3);
			mod->GetAnimator()->Play();
		}
		else
		{
			mod->GetAnimator()->Pause();
			mod->GetAnimator()->SetAnimation(4);
			mod->GetAnimator()->Play();
		}
		
	}

}