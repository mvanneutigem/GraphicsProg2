#pragma once
#include "Scenegraph/GameObject.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"
#include "Components/ClothComponent.h"
#include "Components/TransformComponent.h"

class ControllerComponent;
class CameraComponent;

class JumpCharacter : public GameObject
{
public:
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		JUMP
	};

	JumpCharacter(float radius = 2, float height = 5, float moveSpeed = 0.5f);
	virtual ~JumpCharacter();
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

	GameObject* m_cloth;

	void Jump(bool& succes)
	{
		if (m_Velocity.y < 0 && !m_Launching)
		{
			if (!m_Jumping )
			{

				m_cloth->GetComponent<ClothComponent>();
				cout << "jump" << endl;
				auto mod = this->GetChild<GameObject>()->GetComponent<ModelComponent>();
				mod->GetAnimator()->Pause();
				mod->GetAnimator()->SetAnimation(0);
				mod->GetAnimator()->Play();
				succes = true;
			}
			
			m_Jumping = true;
			m_Velocity.y = 40;
			m_JumpVelocity = 0;
		}

	}

	void SetTimeScale(float t)
	{
		m_TimeScale = t;
		auto mod = this->GetChild<GameObject>()->GetComponent<ModelComponent>();
		if (m_TimeScale < 0.5f)
		{
			mod->GetAnimator()->Pause();
		}
		else
		{
			mod->GetAnimator()->Play();
		}
	}

	void Launch()
	{
		m_pParticleEmitterObject->GetTransform()->Translate(0, 0, 0);
		m_Launching = true;
		m_LaunchTime = 5;
		m_Velocity.y = 50;
	}

	void StopLaunch()
	{
		if(m_Launching)
		{
			m_LaunchTime = 0;
			m_pParticleEmitterObject->GetTransform()->Translate(0, -100, 0);
			m_Launching = false;
		}
	}
	bool m_Launching = false;
protected:

	ControllerComponent* m_pController;

	float m_TimeScale = 1.0f;
	float m_TotalPitch, m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed;
	float m_Radius, m_Height;

	//Running
	float m_MaxRunVelocity,
		m_TerminalVelocity,
		m_Gravity,
		m_RunAccelerationTime,
		m_JumpAccelerationTime,
		m_RunAcceleration,
		m_JumpAcceleration,
		m_RunVelocity,
		m_JumpVelocity,
		m_LaunchTime;

	XMFLOAT3 m_Velocity;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameObject* m_pParticleEmitterObject = new GameObject();
	bool m_Jumping = false;
	float m_JumpTime = 0.0f;
	float m_Rotation = 0;
	JumpCharacter(const JumpCharacter& t);
	JumpCharacter& operator=(const JumpCharacter& t);
};


