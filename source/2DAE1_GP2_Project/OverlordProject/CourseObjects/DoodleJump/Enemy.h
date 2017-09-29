#pragma once
#include "Scenegraph/GameObject.h"
#include "Components/components.h"
#include "PickUp.h"

class Enemy : public PickUp
{
public:
	Enemy(wstring model, wstring collider, int matID, XMFLOAT3 scale = XMFLOAT3{ 1,1,1 });
	~Enemy();
	void Update(const GameContext& gameContext) override;
private:
	bool m_SetAnim = false;
};

