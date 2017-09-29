//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "DoodleScene.h"
#include "Scenegraph\GameObject.h"
#include "Graphics/ModelAnimator.h"
#include "Content\ContentManager.h"
#include "Components\Components.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "JumpCharacter.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow_Outline.h"
#include "Prefabs/FixedCamera.h"
#include "Base/OverlordGame.h"
#include "Platform.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/TextRenderer.h"
#include "PickUp.h"
#include "Enemy.h"
#include "Graphics/Button.h"
#include "../../Materials/Post/PostGrayscale.h"
#include "../../Materials/Post/PostBlur.h"
#include "../../Materials/Post/PostVignette.h"
#include "../../Materials/Post/PostChromAb.h"
#include "Graphics/SpriteRenderer.h"
#include "Base/SoundManager.h"

DoodleScene::DoodleScene(void) :
	GameScene(L"DoodleScene")
{
}


DoodleScene::~DoodleScene(void)
{
	if(!m_Dead)
	{
		SafeDelete( m_pRestart);
		m_pRestart = nullptr;
	}

}

void DoodleScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/fonts/consolas32.fnt");

	//GetPhysxProxy()->EnablePhysxDebugRendering(true);

	gameContext.pShadowMapper->SetLight({ 0, 25, -50 }, { 0.025f,-0.025f, 0.95f });
	//Materials
	//*********
	auto diffMat = new DiffuseMaterial_Shadow_Outline();
	diffMat->SetDiffuseTexture(L"./Resources/Textures/Doodle.jpg");
	diffMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat, 0);
	
	auto diffMat1 = new DiffuseMaterial_Shadow();
	diffMat1->SetDiffuseTexture(L"./Resources/Textures/night_sky_light.jpg");
	diffMat1->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat1, 1);


	auto diffMat3 = new SkinnedDiffuseMaterial_Shadow();
	diffMat3->SetDiffuseTexture(L"./Resources/Textures/Doodle_White.jpg");
	diffMat3->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat3, 3);


	//Character Mesh
	//**************

	auto pCharacterObj = new GameObject();

	//Level Model Component
	m_pCharModel = new ModelComponent(L"./Resources/Meshes/mainChar.ovm");
	m_pCharModel->SetMaterial(3);
	pCharacterObj->AddComponent(m_pCharModel);
	pCharacterObj->GetTransform()->Scale(0.2f, 0.2f, 0.2f);
	pCharacterObj->GetTransform()->Translate(0, -4, 0);
	pCharacterObj->GetTransform()->Rotate(0, 0, 0);

	//jumpcharacter
	m_pChar = new JumpCharacter();
	m_pChar->GetTransform()->Translate(0, 10, 0);
	m_pChar->AddChild(pCharacterObj);
	AddChild(m_pChar);

	//AddChild(pCharacterObj);
	m_pCharModel->GetAnimator()->SetAnimation(0);
	m_pCharModel->GetAnimator()->Play();

	//backdrop
	auto pBackdrop = new GameObject();
	auto backdropModel = new ModelComponent(L"./Resources/Meshes/UnitPlane.ovm");
	backdropModel->SetMaterial(1);

	pBackdrop->AddComponent(backdropModel);
	pBackdrop->GetTransform()->Scale(5.0f, 5.0f, 5.0f);
	pBackdrop->GetTransform()->Rotate(270, 0, 0);
	pBackdrop->GetTransform()->Translate(0, 0, 80);


	auto diffMat6 = new DiffuseMaterial_Shadow_Outline();
	diffMat6->SetDiffuseTexture(L"./Resources/Textures/platform_1_blue.jpg");
	diffMat6->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat6, 2);


	//platform
	int nrOfPlatforms = 8;
	m_Height = 0;
	auto platformObj = new Platform(0, 0);
	AddChild(platformObj);
	platformArr.push_back(platformObj);
	for (int i = 0; i< nrOfPlatforms / 2.0f ; ++i)
	{
		auto seedY = rand() % 6 + 8;
		float localheight = float(m_Height + seedY);
		auto seedX = rand() % 10 + 5;
		auto platformObj2 = new Platform(-float(seedX), localheight, 1, true, i);
		AddChild(platformObj2);
		platformArr.push_back(platformObj2);
		m_Height += 10;

		if(i < 3)
		{
			seedY = rand() % 6 + 8;
			localheight = float(m_Height + seedY);
			seedX = rand() % 15;
			auto platformObj1 = new Platform(float(seedX), localheight); Platform(float(-seedX), localheight, float(1), true, i);
			platformArr.push_back(platformObj1);
			AddChild(platformObj1);
		}
		
	}

	//pickup
	auto diffMat5 = new DiffuseMaterial_Shadow();
	diffMat5->SetDiffuseTexture(L"./Resources/Textures/rocket.jpg");
	diffMat5->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat5, 4);

	m_PickUp = new PickUp(L"./Resources/Meshes/RocketPickup.ovm", L"./Resources/Meshes/RocketPickup.ovpc", 4);
	AddChild(m_PickUp);

	//enemy
	//animated material
	auto diffMat4 = new SkinnedDiffuseMaterial_Shadow();
	diffMat4->SetDiffuseTexture(L"./Resources/Textures/eye_texture.jpg");
	diffMat4->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());

	gameContext.pMaterialManager->AddMaterial(diffMat4, 5);

	m_pEnemy = new Enemy(L"./Resources/Meshes/Eyeball.ovm", L"./Resources/Meshes/Eyeball.ovpc", 5, XMFLOAT3(0.2f,0.2f,0.2f));
	AddChild(m_pEnemy);

	m_pEnemy2 = new Enemy(L"./Resources/Meshes/Eyeball.ovm",  L"./Resources/Meshes/Eyeball.ovpc", 5, XMFLOAT3(0.2f, 0.2f, 0.2f));
	AddChild(m_pEnemy2);

	//cam
	m_pfixedCamera = new FixedCamera();
	AddChild(m_pfixedCamera);
	m_pfixedCamera->GetComponent<TransformComponent>()->Translate(0, 25, -50);
	m_pfixedCamera->GetComponent<TransformComponent>()->Rotate(0, 0, 0);
	m_pfixedCamera->GetComponent<CameraComponent>()->UseOrthographicProjection();
	m_pfixedCamera->GetComponent<CameraComponent>()->SetOrthoSize(50);
	auto gameScene = m_pfixedCamera->GetScene();
	gameScene->SetActiveCamera(m_pfixedCamera->GetComponent<CameraComponent>());
	m_pfixedCamera->AddChild(pBackdrop);

	//target
	m_pTargetSprite = ContentManager::Load<TextureData>(L"./Resources/Textures/Game_UI/Target.png");
	gameContext.pInput->AddInputAction(InputAction(11, Pressed, -1, VK_LBUTTON));

	m_pPlus1kSprite = ContentManager::Load<TextureData>(L"./Resources/Textures/Game_UI/plus1k.png");

	m_pEye = ContentManager::Load<TextureData>(L"./Resources/Textures/Game_UI/B_Eye.png");
	//death screen
	m_pRestart = new Button(L"./Resources/Textures/Game_UI/B_restart.png",PxVec2( 250, 480));
	//m_pRestart->Initialize(gameContext);
	//m_pRestart->SetPressed(true);

	//post
	gameContext.pMaterialManager->AddMaterial_PP(new PostGrayscale(), 0);
	gameContext.pMaterialManager->AddMaterial_PP(new PostBlur(), 1);
	gameContext.pMaterialManager->AddMaterial_PP(new PostVignette(), 3);
	gameContext.pMaterialManager->AddMaterial_PP(new PostChromAb(), 2);
	AddPostProcessingMaterial(2);
	AddPostProcessingMaterial(3);

	//sounds
	auto fmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT fmodres;

	fmodres = fmodSystem->createSound("./Resources/Sounds/death.wav", FMOD_DEFAULT, 0, &m_pSound);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->createSound("./Resources/Sounds/rocket.wav", FMOD_DEFAULT, 0, &m_pSound2);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->createSound("./Resources/Sounds/game.mp3", FMOD_LOOP_NORMAL, 0, &m_pSound3);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->createSound("./Resources/Sounds/click.wav", FMOD_DEFAULT, 0, &m_pSound4);
	SoundManager::ErrorCheck(fmodres);

	fmodres = fmodSystem->playSound(m_pSound3, 0, false, &m_pChannel3);
	m_pChannel3->setPaused(true);
}

void DoodleScene::Update(const GameContext& gameContext)
{
	auto fmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD_RESULT fmodres;
	Logger::LogInfo(L"FPS: " + std::to_wstring(gameContext.pGameTime->GetFPS()));

	if (!m_Started)
		m_pChannel3->setPaused(false);

	m_GameTime += gameContext.pGameTime->GetElapsed();
	m_Level = int(m_GameTime) / 15 + 1;
	if (m_Level > 5)//max level is 5
		m_Level = 5;


	//launch
	if(!m_pChar->m_Launching)
	{
		m_CamSpeed = 20.0f;
	}

	//pickup
	if(m_PickUpTime > 0)
	{
		m_PickUpTime -= gameContext.pGameTime->GetElapsed();
	}
	else
	{
		m_SpawnPickUp = true;
		m_PickUpTime = float( rand() % 10 +15);
	}
	//enemys
	if (m_EnemyTime > 0)
	{
		m_EnemyTime -= gameContext.pGameTime->GetElapsed();
	}
	else
	{
		m_SpawnEnemy = true;
		m_EnemyTime = float((rand() % 10 + 25))/ m_Level;
	}
	if (m_Enemy2Time > 0)
	{
		m_Enemy2Time -= gameContext.pGameTime->GetElapsed();
	}
	else
	{
		m_SpawnEnemy2 = true;
		m_Enemy2Time = float((rand() % 10 + 25)) / m_Level;
	}

	//raycast picking
	if (gameContext.pInput->IsActionTriggered(11))
	{
		fmodres = fmodSystem->playSound(m_pSound4, 0, false, &m_pChannel4);
		Logger::LogFormat(LogLevel::Info, L"Clicked");
		auto hitObject = gameContext.pCamera->Pick(gameContext);
		if (hitObject != nullptr)
		{
			wstring info = L"Picked " + hitObject->GetTag();
			if(hitObject->GetTag() == L"Enemy")
			{
				//kill enemy
				Enemy* ch = dynamic_cast<Enemy*> (hitObject->GetParent());
				ch->SetPosition(0, -20);
				m_Score += 1000;
				++m_Eyes;
				m_1kTime = 0.5f;
				m_1KPos.x = float( gameContext.pInput->GetMousePosition().x )- TARGETDIM;
				m_1KPos.y = float( gameContext.pInput->GetMousePosition().y )- TARGETDIM;
			}
			Logger::LogFormat(LogLevel::Info, info.c_str());
		}
	}

	//pickups
	m_PickUp->Update(gameContext);
	if(m_PickUp->TriggeredByCharacter())
	{
		fmodres = fmodSystem->playSound(m_pSound2, 0, false, &m_pChannel2);
		m_PickUp->SetPosition(0, -20);//out of screen
		m_pChar->Launch();
		m_CamSpeed = 50.0f;
	}

	//enemy
	m_pEnemy->Update(gameContext);
	m_pEnemy2->Update(gameContext);
	if ((m_pEnemy->TriggeredByCharacter() || m_pEnemy2->TriggeredByCharacter()) && !m_pChar->m_Launching )
	{
		m_pEnemy->SetPosition(0, -20);//out of screen
		m_pEnemy2->SetPosition(0, -20);
		AddPostProcessingMaterial(0);
		AddPostProcessingMaterial(1);
		m_pChar->SetTimeScale(0.0f);
		m_Dead = true;
		AddChild(m_pRestart);
	}

	//cam
	if (m_pfixedCamera->GetTransform()->GetPosition().y < m_pChar->GetTransform()->GetPosition().y)
	{
		auto pos = m_pfixedCamera->GetTransform()->GetPosition();
		pos.y += m_CamSpeed * gameContext.pGameTime->GetElapsed();
		m_pfixedCamera->GetTransform()->Translate(pos);
		gameContext.pShadowMapper->SetLight({ pos.x ,pos.y,pos.z}, { 0.025f,-0.025f, 0.95f });
		m_Score += int(1000 * gameContext.pGameTime->GetElapsed());
	}

	//platforms
	for(unsigned int i =0; i < platformArr.size(); ++i)
	{
		auto pos = platformArr[i]->GetPlatformObj()->GetTransform()->GetPosition();
		if (pos.y < m_pfixedCamera->GetTransform()->GetPosition().y - 25)
		{
			auto seedY = rand() % 5 + 8;
			auto localheight = m_Height + seedY;
			auto seedX = rand() % 30 - 15;
			platformArr[i]->SetPosition(float(seedX), float(localheight));
			m_Height += 5;

			//pickup
			if(m_SpawnPickUp)
			{
				m_PickUp->SetPosition(float(seedX),float( localheight + 1));
				m_SpawnPickUp = false;
			}
			//enemy
			else if(m_SpawnEnemy)
			{
				m_pEnemy->SetPosition(float(seedX), float(localheight + 4));
				m_SpawnEnemy = false;
			}
			else if (m_SpawnEnemy2)//else if to prevent double spawning
			{
				m_pEnemy2->SetPosition(float(seedX), float( localheight + 4));
				m_SpawnEnemy2 = false;
			}
		}
	}

	if (m_pfixedCamera->GetTransform()->GetPosition().y - 40 > m_pChar->GetTransform()->GetPosition().y && !m_Dead)
	{
		fmodres = fmodSystem->playSound(m_pSound, 0, false, &m_pChannel);
		SoundManager::ErrorCheck(fmodres);
		//death!
		AddPostProcessingMaterial(0);
		AddPostProcessingMaterial(1);
		m_pChar->SetTimeScale(0.0f);
		m_Dead = true;
		AddChild(m_pRestart);
	}

	if(m_Dead)
	{
		m_pRestart->Update(gameContext);
		if(m_pRestart->MouseClicked())
		{
			Logger::LogInfo(L"Restart");
			Restart(gameContext);
		}
		if(!m_Paused)
		{
			m_pChannel3->setPaused(true);
			m_Paused = true;
		}
		
	}
}

void DoodleScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_Dead)
	{
		if (m_Score > m_HighScore)
			m_HighScore = m_Score;
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"You Died!", { 250,350 }, XMFLOAT4{ Colors::White });
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Score:", { 250,390 }, XMFLOAT4{ Colors::White });

		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(m_Score), { 250,440 }, XMFLOAT4{ Colors::White });
		
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"HighScore:", { 250,540 }, XMFLOAT4{ Colors::White });

		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(m_HighScore), { 250,590 }, XMFLOAT4{ Colors::White });

		m_pRestart->Draw(gameContext);
	}
	else
	{
		if(m_1kTime > 0)
		{
			SpriteRenderer::GetInstance()->Draw(m_pPlus1kSprite, m_1KPos);
			m_1kTime -= gameContext.pGameTime->GetElapsed();
		}

		SpriteRenderer::GetInstance()->Draw(m_pEye, XMFLOAT2(500,12));
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont,L"x" + std::to_wstring(m_Eyes), { 540,10 }, XMFLOAT4{ Colors::White });


		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, std::to_wstring(m_Score), { 10,10 }, XMFLOAT4{ Colors::White });
		//mouse
		XMFLOAT2 pos;
		pos.x = float(gameContext.pInput->GetMousePosition().x - TARGETDIM);
		pos.y = float(gameContext.pInput->GetMousePosition().y - TARGETDIM);
		SpriteRenderer::GetInstance()->Draw(m_pTargetSprite, pos);
	}
}

void DoodleScene::Restart(GameContext gameContext)
{
	m_Paused = false;
	m_pChar->StopLaunch();
	m_GameTime = 0;
	RemovePostProcessingMaterial(0);
	RemovePostProcessingMaterial(1);

	m_Eyes = 0;
	m_Score = 0;
	m_pfixedCamera->GetComponent<TransformComponent>()->Translate(0, 25, -50);
	auto pos = m_pfixedCamera->GetTransform()->GetPosition();
	gameContext.pShadowMapper->SetLight({ pos.x ,pos.y,pos.z }, { 0.025f,-0.025f, 0.95f });
	m_pChar->GetTransform()->Translate(0, 10, 0);

	m_Height = 0;
	platformArr[0]->SetPosition(0, 0);
	for (unsigned int i = 1; i< platformArr.size(); ++i)
	{
		auto seedY = rand() % 6 + m_Height;
		auto seedX = rand() % 30 -15;
		platformArr[i]->SetPosition(float(seedX), float(seedY));
		m_Height += 5;
	}

	m_Dead = false;
	m_pChar->SetTimeScale(1.0f);
	RemoveChild(m_pRestart, false);
}