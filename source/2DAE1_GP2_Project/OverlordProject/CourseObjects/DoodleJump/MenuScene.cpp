#include "stdafx.h"
#include "MenuScene.h"
#include <Physx/PhysxProxy.h>
#include "Prefabs/FixedCamera.h"
#include "Components/Components.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Graphics/SpriteRenderer.h"
#include "Content/ContentManager.h"
#include "Graphics/TextRenderer.h"
#include "../../Materials/Post/PostVignette.h"
#include "Graphics/Button.h"
#include "Scenegraph/SceneManager.h"
#include "Base/SoundManager.h"


MenuScene::MenuScene() :
	GameScene(L"MenuScene")
{
}

MenuScene::~MenuScene()
{
}
void MenuScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	UNREFERENCED_PARAMETER(gameContext);
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/fonts/consolas32.fnt");

	m_pBackDrop = ContentManager::Load<TextureData>(L"./Resources/Textures/Game_UI/menuScreen.jpg");

	//cam
	auto pfixedCamera = new FixedCamera();
	AddChild(pfixedCamera);
	pfixedCamera->GetComponent<TransformComponent>()->Translate(0, 25, -1);
	pfixedCamera->GetComponent<TransformComponent>()->Rotate(0, 0, 0);
	pfixedCamera->GetComponent<CameraComponent>()->UseOrthographicProjection();
	pfixedCamera->GetComponent<CameraComponent>()->SetOrthoSize(50);
	auto gameScene = pfixedCamera->GetScene();
	gameScene->SetActiveCamera(pfixedCamera->GetComponent<CameraComponent>());

	gameContext.pMaterialManager->AddMaterial_PP(new PostVignette(), 3);
	AddPostProcessingMaterial(3);

	m_pStartButton = new Button(L"./Resources/Textures/Game_UI/B_Start.png", PxVec2(50, 230));
	m_pStartButton->Initialize(gameContext);
	m_pStartButton->SetPressed(true);
	AddChild(m_pStartButton);

	auto fmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT fmodres;

	fmodres = fmodSystem->createSound("./Resources/Sounds/menu.mp3", FMOD_LOOP_NORMAL, 0, &m_pSound);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->createSound("./Resources/Sounds/button.wav", FMOD_DEFAULT, 0, &m_pSound2);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->playSound(m_pSound, 0, false, &m_pChannel);

}

void MenuScene::Update(const GameContext& gameContext)
{
	auto fmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT fmodres;

	m_pStartButton->Update(gameContext);
	if(m_pStartButton->MouseClicked())
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"DoodleScene");
		m_pChannel->setPaused(true);
		fmodres = fmodSystem->playSound(m_pSound2, 0, false, &m_pChannel2);
	}
}

void MenuScene::Draw(const GameContext& gameContext)
{

	SpriteRenderer::GetInstance()->Draw(m_pBackDrop,XMFLOAT2(-215,-80));
	m_pStartButton->Draw(gameContext);
}
