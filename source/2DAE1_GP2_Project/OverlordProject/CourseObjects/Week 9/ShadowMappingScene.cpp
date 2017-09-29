//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ShadowMappingScene.h"
#include "Scenegraph\GameObject.h"
#include "Graphics/ModelAnimator.h"
#include "Content\ContentManager.h"
#include "Components\Components.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"

//shadowmaprenderer 
//begin
//reset to null check the code already in post, set rendertarget (shadowrt),  set light from shadowmat, clear rendertarget.
//end
//setrendertarget(nullpt)
//draw
//use meshfilter to fill vertex buffer

//copy shaders _shadow
//diffuse and skinneddiffuse
//m_pShadowSRW_Variabele
// create posnormtex3d_shadow.fx which accepts a shadowmap, VS: output.lpos = nul ( pos, worldviewproj_light)
//float4 lpos in the output
//evaluate shadowmap: lpos.xyz /= lpos.w check if lpos is inbetween -1 to 1 on x y and z if not return 0
//transform clip space coord//lpos.x = lpos.x / 2 + 0.5 , -2 for y
//lpos.z -= gshadowmapbias
//shadowmapdepth = gshadowmap.sample(sampoint, lpos.xy).r

//filtering in shader
//pcf samplin for shadowmap sum = 0; float x, y;
//for(y = -1.5; y <= 1.5; y +=1)
//	for x
//		 sum +=  shadowmpasample
//shadowfactor /=

//ps return (color_rgb *shadowvalue, color_a) at end

ShadowMappingScene::ShadowMappingScene(void) :
	GameScene(L"ShadowMappingScene")
{
}


ShadowMappingScene::~ShadowMappingScene(void)
{
}

void ShadowMappingScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	gameContext.pShadowMapper->SetLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	//Materials
	//*********
	auto diffMat = new SkinnedDiffuseMaterial_Shadow();
	diffMat->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	diffMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat, 0);
	

	auto diffMat2 = new DiffuseMaterial_Shadow();
	diffMat2->SetDiffuseTexture(L"./Resources/Textures/GroundBrick.jpg");
	diffMat2->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat2, 1);

	//Ground Mesh
	//***********

	auto pGroundObj = new GameObject();
	auto pGroundModel = new ModelComponent(L"./Resources/Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(1);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);

	AddChild(pGroundObj);

	//Character Mesh
	//**************

	auto pCharacterObj = new GameObject();

	//Level Model Component
	m_pKnightModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pKnightModel->SetMaterial(0);
	pCharacterObj->AddComponent(m_pKnightModel);

	AddChild(pCharacterObj);

	m_pKnightModel->GetAnimator()->SetAnimation(0);
	m_pKnightModel->GetAnimator()->Play();
	pCharacterObj->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	//Input
	//*****
	gameContext.pInput->AddInputAction(InputAction(0, Pressed, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(1, Pressed, VK_UP));
}

void ShadowMappingScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (gameContext.pInput->IsActionTriggered(0))
	{
		gameContext.pShadowMapper->SetLight(gameContext.pCamera->GetTransform()->GetPosition(), gameContext.pCamera->GetTransform()->GetForward());

		static_cast<SkinnedDiffuseMaterial_Shadow*>(gameContext.pMaterialManager->GetMaterial(0))->SetLightDirection(gameContext.pCamera->GetTransform()->GetForward());
		static_cast<DiffuseMaterial_Shadow*>(gameContext.pMaterialManager->GetMaterial(1))->SetLightDirection(gameContext.pCamera->GetTransform()->GetForward());
	}
	if (gameContext.pInput->IsActionTriggered(1))
	{
		m_pKnightModel->GetTransform()->Translate(20.0f, 0, 0);
	}
}

void ShadowMappingScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
}
