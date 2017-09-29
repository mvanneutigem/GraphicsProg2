#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class Material;

class PickerScene : public GameScene
{
public:
	PickerScene(void);
	virtual ~PickerScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	GameObject* m_pChair;
	GameObject* m_pGround;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PickerScene(const PickerScene &obj);
	PickerScene& operator=(const PickerScene& obj);
};

