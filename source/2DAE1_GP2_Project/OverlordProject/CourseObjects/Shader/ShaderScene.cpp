#include "stdafx.h"
#include "ShaderScene.h"
#include "../../Materials/shader/CrumbleMaterial.h"
#include "Components/Components.h"
#include "Physx/PhysxProxy.h"
#include <Scenegraph/GameObject.h>
#include "Graphics/Button.h"
#include "Content/ContentManager.h"
#include "Graphics/TextRenderer.h"
#include "../../Materials/ColorMaterial.h"

#define FPS_COUNTER 1

ShaderScene::ShaderScene(void) :
	GameScene(L"ShaderScene"),
	m_FpsInterval(FPS_COUNTER)
{
}

ShaderScene::~ShaderScene()
{
	SafeDelete(m_pButtonPlay);
	SafeDelete(m_pButtonPause);
	SafeDelete(m_pButtonStop);

	SafeDelete(m_pButtonVoxelSizeUp);
	SafeDelete(m_pButtonVoxelSizeDown);

	SafeDelete(m_pButtonFallSeedXUp);
	SafeDelete(m_pButtonFallSeedXDown);

	SafeDelete(m_pButtonThicknessUp);
	SafeDelete(m_pButtonThicknessDown);

	SafeDelete(m_pButtonFallSeedYUp);
	SafeDelete(m_pButtonFallSeedYDown);

	SafeDelete(m_pButtonFallMultiplierUp);
	SafeDelete(m_pButtonFallMultiplierDown);

	SafeDelete(m_pButtonGridSizeUp);
	SafeDelete(m_pButtonGridSizeDown);
}

void ShaderScene::Initialize(const GameContext & gameContext)
{
	cout << "Init" << endl;
	UNREFERENCED_PARAMETER(gameContext);
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/fonts/consolas32.fnt");

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	m_pCrumbleMaterial = new CrumbleMaterial();
	m_pCrumbleMaterial->SetDiffuseTexture(L"./Resources/Textures/wall_texture.jpg");
	gameContext.pMaterialManager->AddMaterial(m_pCrumbleMaterial, 0);
	//auto col = new ColorMaterial(false);
	//gameContext.pMaterialManager->AddMaterial(col, 0);

	auto pModel = new ModelComponent(L"./Resources/Meshes/UnitSphere.ovm");
	pModel->SetMaterial(0);
	auto obj = new GameObject();
	obj->AddComponent(pModel);
	AddChild(obj);

	//UI

	//time
	m_pButtonPlay = new Button(L"./Resources/Textures/icon_play.png", PxVec2(20, 5));
	m_pButtonPlay->Initialize(gameContext);

	m_pButtonPause = new Button(L"./Resources/Textures/icon_pause.png", PxVec2(60, 5));
	m_pButtonPause->Initialize(gameContext);

	m_pButtonStop = new Button(L"./Resources/Textures/icon_stop.png", PxVec2(100, 5));
	m_pButtonStop->Initialize(gameContext);

	//voxelsize
	m_pButtonVoxelSizeUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 40));
	m_pButtonVoxelSizeUp->Initialize(gameContext);

	m_pButtonVoxelSizeDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 80));
	m_pButtonVoxelSizeDown->Initialize(gameContext);

	//thickness
	m_pButtonThicknessUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 120));
	m_pButtonThicknessUp->Initialize(gameContext);

	m_pButtonThicknessDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 160));
	m_pButtonThicknessDown->Initialize(gameContext);

	//crumble
	m_pButtonFallSeedXUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 200));
	m_pButtonFallSeedXUp->Initialize(gameContext);

	m_pButtonFallSeedXDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 240));
	m_pButtonFallSeedXDown->Initialize(gameContext);

	//gravity
	m_pButtonFallSeedYUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 280));
	m_pButtonFallSeedYUp->Initialize(gameContext);

	m_pButtonFallSeedYDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 320));
	m_pButtonFallSeedYDown->Initialize(gameContext);

	//fallmultiplier
	m_pButtonFallMultiplierUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 360));
	m_pButtonFallMultiplierUp->Initialize(gameContext);

	m_pButtonFallMultiplierDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 400));
	m_pButtonFallMultiplierDown->Initialize(gameContext);

	//grid size
	m_pButtonGridSizeUp = new Button(L"./Resources/Textures/icon_up.png", PxVec2(200, 440));
	m_pButtonGridSizeUp->Initialize(gameContext);

	m_pButtonGridSizeDown = new Button(L"./Resources/Textures/icon_down.png", PxVec2(200, 480));
	m_pButtonGridSizeDown->Initialize(gameContext);

	//cam
	gameContext.pCamera->GetTransform()->Translate(0, 3, -5);
}

void ShaderScene::Update(const GameContext & gameContext)
{
	//fall multiplier
	m_pButtonFallMultiplierUp->Update(gameContext);
	m_pButtonFallMultiplierDown->Update(gameContext);
	if (m_pButtonFallMultiplierUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallMultiplier();
		size += 0.05f;
		m_pCrumbleMaterial->SetFallMultiplier(size);
	}
	if (m_pButtonFallMultiplierDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallMultiplier();
		size -= 0.05f;
		m_pCrumbleMaterial->SetFallMultiplier(size);
	}

	//grid size
	m_pButtonGridSizeUp->Update(gameContext);
	m_pButtonGridSizeDown->Update(gameContext);
	if (m_pButtonGridSizeUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetGridSize();
		size += 0.01f;
		m_pCrumbleMaterial->SetGridSize(size);
	}
	if (m_pButtonGridSizeDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetGridSize();
		size -= 0.01f;
		m_pCrumbleMaterial->SetGridSize(size);
	}

	//gravity
	m_pButtonFallSeedYUp->Update(gameContext);
	m_pButtonFallSeedYDown->Update(gameContext);
	if (m_pButtonFallSeedYUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallSeedY();
		size += 0.05f;
		m_pCrumbleMaterial->SetFallSeedY(size);
	}
	if (m_pButtonFallSeedYDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallSeedY();
		size -= 0.05f;
		m_pCrumbleMaterial->SetFallSeedY(size);
	}

	//crumble
	m_pButtonFallSeedXUp->Update(gameContext);
	m_pButtonFallSeedXDown->Update(gameContext);
	if (m_pButtonFallSeedXUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallSeedX();
		size += 0.05f;
		m_pCrumbleMaterial->SetFallSeedX(size);
	}
	if (m_pButtonFallSeedXDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetFallSeedX();
		size -= 0.05f;
		m_pCrumbleMaterial->SetFallSeedX(size);
	}

	//thickness
	m_pButtonThicknessDown->Update(gameContext);
	m_pButtonThicknessUp->Update(gameContext);
	if (m_pButtonThicknessUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetThickness();
		size += 0.005f;
		m_pCrumbleMaterial->SetThickness(size);
	}
	if (m_pButtonThicknessDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetThickness();
		size -= 0.005f;
		m_pCrumbleMaterial->SetThickness(size);
	}

	//time
	m_pButtonPlay->Update(gameContext);
	m_pButtonPause->Update(gameContext);
	m_pButtonStop->Update(gameContext);
	if (m_pButtonPlay->MouseClicked())
	{
		m_pCrumbleMaterial->PauseTime(false);
	}
	if (m_pButtonPause->MouseClicked())
	{
		m_pCrumbleMaterial->PauseTime(true);
	}
	if (m_pButtonStop->MouseClicked())
	{
		m_pCrumbleMaterial->SetTime(0.0f);
	}

	//blocksize
	m_pButtonVoxelSizeDown->Update(gameContext);
	m_pButtonVoxelSizeUp->Update(gameContext);
	if (m_pButtonVoxelSizeUp->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetBlockSize();
		size += 0.001f;
		m_pCrumbleMaterial->SetBlockSize(size);
	}
	if(m_pButtonVoxelSizeDown->MouseClicked())
	{
		float size = m_pCrumbleMaterial->GetBlockSize();
		size -= 0.001f;
		m_pCrumbleMaterial->SetBlockSize(size);
	}

}

void ShaderScene::Draw(const GameContext & gameContext)
{
	//m_FpsInterval += gameContext.pGameTime->GetElapsed();
	//if (m_FpsInterval >= FPS_COUNTER)
	//{
	//	m_FpsInterval -= FPS_COUNTER;
	//	Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	//}
	cout << "FPS: " << gameContext.pGameTime->GetFPS()<< endl;
	
	//fall multiplier
	m_pButtonFallMultiplierUp->Draw(gameContext);
	m_pButtonFallMultiplierDown->Draw(gameContext);

	//grid size
	m_pButtonGridSizeUp->Draw(gameContext);
	m_pButtonGridSizeDown->Draw(gameContext);

	//gravity
	m_pButtonFallSeedYUp->Draw(gameContext);
	m_pButtonFallSeedYDown->Draw(gameContext);

	//crumble
	m_pButtonFallSeedXUp->Draw(gameContext);
	m_pButtonFallSeedXDown->Draw(gameContext);

	//thickness
	m_pButtonThicknessDown->Draw(gameContext);
	m_pButtonThicknessUp->Draw(gameContext);

	//time
	m_pButtonPlay->Draw(gameContext);
	m_pButtonPause->Draw(gameContext);
	m_pButtonStop->Draw(gameContext);

	//blocksize
	m_pButtonVoxelSizeDown->Draw(gameContext);
	m_pButtonVoxelSizeUp->Draw(gameContext);

	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Block Size", { 20,60 }, XMFLOAT4{ Colors::Black });
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Thickness", { 20,140 }, XMFLOAT4{ Colors::Black });
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Fall Seed X", { 20,220 }, XMFLOAT4{ Colors::Black });
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Fall Seed Y", { 20,300 }, XMFLOAT4{ Colors::Black });
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Fall Mult", { 20,380 }, XMFLOAT4{ Colors::Black });
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Grid size", { 20,460 }, XMFLOAT4{ Colors::Black });
}
