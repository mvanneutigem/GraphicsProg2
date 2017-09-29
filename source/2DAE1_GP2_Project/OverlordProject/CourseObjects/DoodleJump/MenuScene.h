#pragma once
#include "Scenegraph/GameScene.h"
class SpriteFont;
class Button;

class MenuScene : public GameScene
{
public:
	MenuScene();
	virtual ~MenuScene();

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	Button* m_pStartButton = nullptr;
	TextureData* m_pBackDrop = nullptr;
	SpriteFont* m_pSpriteFont = nullptr;
	FMOD::Sound *m_pSound, *m_pSound2;
	FMOD::Channel *m_pChannel, *m_pChannel2;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MenuScene(const MenuScene &obj);
	MenuScene& operator=(const MenuScene& obj);
};

