#pragma once
#include "Scenegraph/GameObject.h"
#include "Components/components.h"

class PickUp : public GameObject
{
public:
	PickUp(wstring model, wstring collider, int matID, XMFLOAT3 scale = XMFLOAT3{1,1,1} );
	~PickUp();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;

	void SetPosition(float x, float y)// const
	{
		m_GameObject->GetComponent<RigidBodyComponent>()->SetKinematic(false);
		m_GameObject->GetTransform()->Translate(x, y, 0);
		m_GameObject->GetComponent<RigidBodyComponent>()->SetKinematic(true);
	};

	bool TriggeredByCharacter();

protected:
	GameObject* m_GameObject = nullptr;
	bool m_Triggered = false;

private:
	wstring m_Model;
	wstring m_Collider;
	int m_ID;
	XMFLOAT3 m_Scale;

};

