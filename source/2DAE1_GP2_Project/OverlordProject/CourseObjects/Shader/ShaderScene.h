#pragma once
#include "Scenegraph/GameScene.h"
class SpriteFont;
class Button;
class CrumbleMaterial;
class ShaderScene : public GameScene
{
public:
	ShaderScene(void);
	virtual ~ShaderScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	//ui
	Button* m_pButtonPlay = nullptr;
	Button* m_pButtonPause = nullptr;
	Button* m_pButtonStop = nullptr;

	Button* m_pButtonVoxelSizeUp = nullptr;
	Button* m_pButtonVoxelSizeDown = nullptr;

	Button* m_pButtonThicknessUp = nullptr;
	Button* m_pButtonThicknessDown = nullptr;

	Button* m_pButtonFallSeedXUp = nullptr;
	Button* m_pButtonFallSeedXDown = nullptr;

	Button* m_pButtonFallSeedYUp = nullptr;
	Button* m_pButtonFallSeedYDown = nullptr;

	Button* m_pButtonFallMultiplierUp = nullptr;
	Button* m_pButtonFallMultiplierDown = nullptr;

	Button* m_pButtonGridSizeUp = nullptr;
	Button* m_pButtonGridSizeDown = nullptr;

	SpriteFont* m_pSpriteFont = nullptr;
	CrumbleMaterial* m_pCrumbleMaterial = nullptr;

	float m_FpsInterval;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ShaderScene(const ShaderScene &obj);
	ShaderScene& operator=(const ShaderScene& obj);
};

