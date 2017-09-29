#pragma once
#include "Scenegraph/GameScene.h"

class ClothScene : public GameScene
{
public:
	ClothScene(void);
	virtual ~ClothScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ClothScene(const ClothScene &obj);
	ClothScene& operator=(const ClothScene& obj);
};

