
#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class Material;

class SkyBoxScene : public GameScene
{
public:
	SkyBoxScene(void);
	virtual ~SkyBoxScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	GameObject* m_pGround;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxScene(const SkyBoxScene &obj);
	SkyBoxScene& operator=(const SkyBoxScene& obj);
};

