//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Character.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}


Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//PhysX
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	// Create controller
	m_pController = new ControllerComponent(bouncyMaterial);
	AddComponent(m_pController);

	// Add a fixed camera as child
	auto fixedCamera = new FixedCamera();
	m_pCamera = new CameraComponent();
	fixedCamera->AddComponent(m_pCamera);
	AddChild(fixedCamera);

	// Register all Input Actions
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		JUMP
	};

	gameContext.pInput->AddInputAction(InputAction(LEFT, Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(RIGHT, Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(FORWARD, Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(BACKWARD, Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(JUMP, Pressed, VK_SPACE));

}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	// Set the camera as active
	m_pCamera->SetActive();

	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
}


void Character::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//Update the character (Camera rotation, Character Movement, Character Gravity)

	if (m_pCamera->IsActive())
	{
		auto dTime = gameContext.pGameTime->GetElapsed();
		//HANDLE INPUT
		XMFLOAT3 move = XMFLOAT3(0, 0, 0);
		move.z = gameContext.pInput->IsActionTriggered(FORWARD) ? 1.0f : 0.0f;
		if (move.z == 0) move.z = -(gameContext.pInput->IsActionTriggered(BACKWARD) ? 1.0f : 0.0f);

		move.x = gameContext.pInput->IsActionTriggered(RIGHT) ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsActionTriggered(LEFT) ? 1.0f : 0.0f);

		//CALCULATE TRANSFORMS
		XMFLOAT3 forward = GetTransform()->GetForward();
		forward.y = 0;
		//normalize forwardvector
		float forwardLength = sqrt(forward.x * forward.x + forward.z * forward.z);
		forward.x /= forwardLength;
		forward.z /= forwardLength;

		XMFLOAT3 right = GetTransform()->GetRight();

		auto moveZ = XMFLOAT3(move.z * forward.x, move.z *forward.y , move.z * forward.z);
		auto moveX = XMFLOAT3(move.x * right.x, move.x *right.y, move.x * right.z);
		move = XMFLOAT3(moveZ.x + moveX.x , moveZ.y + moveX.y, moveZ.z + moveX.z);
		auto direction = move;

		if (move.x != 0 && move.z != 0)
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

		auto IsOnGround = m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN;
		if (IsOnGround != PxControllerCollisionFlag::eCOLLISION_DOWN )
		{
			cout << "not on ground" << endl;
			m_JumpVelocity -= m_JumpAcceleration * dTime;
			if (m_JumpVelocity > m_TerminalVelocity)
				m_JumpVelocity = m_TerminalVelocity;
			m_JumpVelocity -= m_Gravity * dTime;

		}
		else if (gameContext.pInput->IsActionTriggered(JUMP))
		{
			m_JumpVelocity = 0;
			m_Velocity.y = 200;
		}
		else
		{
			m_Velocity.y = 0;
		}
			
		m_Velocity.y += m_JumpVelocity;

		auto movement = XMFLOAT3{ m_Velocity.x * dTime,  m_Velocity.y * dTime , m_Velocity.z * dTime };

		m_pController->Move(movement);

		//camera rotation
		XMFLOAT2 look = XMFLOAT2(0, 0);
		auto mouseMove = gameContext.pInput->GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);

		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();

		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
	}
}