#pragma once
#include "Scenegraph/GameScene.h"

class UberShaderScene : public GameScene
{
public:
	UberShaderScene(void);
	virtual ~UberShaderScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pTeapot;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberShaderScene(const UberShaderScene &obj);
	UberShaderScene& operator=(const UberShaderScene& obj);
};