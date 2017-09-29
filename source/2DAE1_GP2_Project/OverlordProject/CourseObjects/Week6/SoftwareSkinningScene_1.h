#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"
#include "../../CourseObjects/Week6/BoneObject.h"

class Material;

class SoftwareSkinningScene_1 : public GameScene
{
public:
	SoftwareSkinningScene_1(void);
	virtual ~SoftwareSkinningScene_1(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	BoneObject *m_pBone0, *m_pBone1; 
	float m_BoneRotation; 
	int m_RotationSign;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoftwareSkinningScene_1(const SoftwareSkinningScene_1 &obj);
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& obj);
};

