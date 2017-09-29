#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"
#include "../../CourseObjects/Week6/BoneObject.h"
#include "Components/MeshDrawComponent.h"

class Material;

class SoftwareSkinningScene_3 : public GameScene
{
public:
	SoftwareSkinningScene_3(void);
	virtual ~SoftwareSkinningScene_3(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	void CreateMesh(float length);
	MeshDrawComponent* m_pMeshDrawer;
	vector<SkinnedVertex> m_SkinnedVertices;

	BoneObject *m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoftwareSkinningScene_3(const SoftwareSkinningScene_3 &obj);
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& obj);
};

