
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SoftwareSkinningScene_1.h"
#include "Physx/PhysxProxy.h"
#include "../../Materials/ColorMaterial.h"
#include "Components/TransformComponent.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1(void) :
	GameScene(L"SoftwareSkinningsCene")
{
	m_pBone0 = nullptr;
	m_pBone1 = nullptr;
	m_BoneRotation = 0;
	m_RotationSign = 1;
}


SoftwareSkinningScene_1::~SoftwareSkinningScene_1(void)
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto colMat = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colMat, 1);

	auto root = new GameObject();
	m_pBone0 = new BoneObject(0, 1, 15);
	m_pBone1 = new BoneObject(1, 1, 15);
	m_pBone0->AddBone(m_pBone1);
	root->AddChild(m_pBone0);
	root->GetTransform()->Rotate(0, -90, 0);
	AddChild(root);
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_BoneRotation += m_RotationSign * 45 * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45)
		m_RotationSign = 1;
	if (m_RotationSign > 0 && m_BoneRotation >= 45)
		m_RotationSign = -1;

	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0, 0);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation*2, 0, 0);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}