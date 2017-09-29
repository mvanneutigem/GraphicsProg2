#pragma once
#include "Scenegraph/GameObject.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"

class Platform : public GameObject
{
public:
	Platform(float x = 0, float y = 0, float size = 1, bool breakable = false, int ID = 0);
	~Platform();

	void SetPosition(float x, float y)
	{
		m_Reset = true;
		platform->GetComponent<RigidBodyComponent>()->SetKinematic(false);
		m_Position = PxVec3(x, y, 0);
		platform->GetTransform()->Translate(x,y,0);
		platform->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	};

	GameObject* GetPlatformObj() { return platform; }

	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
private:
	PxVec3 m_Position;
	PxVec3 m_Scale;
	bool m_Breakable;
	int m_BreakableID = 20;
	bool m_Reset = false;
	bool m_Broken = false;

	GameObject* platform = nullptr;
};

