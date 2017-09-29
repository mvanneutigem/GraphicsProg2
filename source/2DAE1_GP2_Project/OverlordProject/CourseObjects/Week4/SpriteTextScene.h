#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"
#include "Graphics/SpriteFont.h"

class Material;

class SpriteTextScene : public GameScene
{
public:
	SpriteTextScene(void);
	virtual ~SpriteTextScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	SpriteFont* m_pSpriteFont = nullptr;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteTextScene(const SpriteTextScene &obj);
	SpriteTextScene& operator=(const SpriteTextScene& obj);
};