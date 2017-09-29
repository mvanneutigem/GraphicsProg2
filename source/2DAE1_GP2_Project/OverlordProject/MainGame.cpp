//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"

//#define W1
//#define W2
//#define W3
//#define W4
//#define W5
//#define W6
//#define W7
//#define W8
//#define W9
//#define W10
#define final
//#define shader

#ifdef W1
#include "CourseObjects/Week 1/TestScene.h"
#include "CourseObjects/Week 1/ComponentTestScene.h"
#include "CourseObjects/Week 1/pong.h"
#endif

#ifdef W2
#include "CourseObjects/Week 2/ModelTestScene.h"
#include "CourseObjects/Week 2/CharacterTest.h"
#endif

#ifdef W3
#include "CourseObjects/Week3/SpikeyScene.h"
#include "CourseObjects/Week3/SpriteTestScene.h"
#include "CourseObjects/Week3/UberShaderScene.h"
#endif

#ifdef W4
#include "CourseObjects/Week4/PickerScene.h"
#include "CourseObjects/Week4/SpriteTextScene.h"
#endif

#ifdef W5
#include "CourseObjects/Week5/SkyBoxScene.h"
#include "CourseObjects/Week5/TerrainTestScene.h"
#endif

#ifdef W6
#include "CourseObjects/Week6/SoftwareSkinningScene_1.h"
#include "CourseObjects/Week6/SoftwareSkinningScene_2.h"
#include "CourseObjects/Week6/SoftwareSkinningScene_3.h"
#endif

#ifdef W7
#include "CourseObjects/Week7/HardwareSkinningScene.h"
#endif

#ifdef W8
#include "CourseObjects/Week8/ClothScene.h"
#include "CourseObjects/Week8/PostProcessingScene.h"
#endif

#ifdef W9
#include "CourseObjects/Week 9/ShadowMappingScene.h"
#endif

#ifdef W10
#include "CourseObjects/Week 10/ParticleTestScene.h"
#endif

#ifdef final
#include "CourseObjects/DoodleJump/MenuScene.h"
#include "CourseObjects/DoodleJump/DoodleScene.h"
#endif

#ifdef shader
#include "CourseObjects/Shader/ShaderScene.h"
#endif

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{

#ifdef W1
	SceneManager::GetInstance()->AddGameScene(new TestScene());
	SceneManager::GetInstance()->AddGameScene(new ComponentTestScene());
	SceneManager::GetInstance()->AddGameScene(new Pong());
#endif

#ifdef W2
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTest());

	//SceneManager::GetInstance()->SetActiveGameScene(L"ModelTestScene");
#endif
#ifdef W3
	SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	SceneManager::GetInstance()->AddGameScene(new UberShaderScene());
#endif
#ifdef W4
	SceneManager::GetInstance()->AddGameScene(new PickerScene());
	SceneManager::GetInstance()->AddGameScene(new SpriteTextScene());
#endif
#ifdef W5
	SceneManager::GetInstance()->AddGameScene(new SkyBoxScene());
	SceneManager::GetInstance()->AddGameScene(new TerrainTestScene());
#endif
#ifdef W6
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());
#endif
#ifdef W7
	SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
#endif
#ifdef W8
	//SceneManager::GetInstance()->AddGameScene(new ClothScene());
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
#endif
#ifdef W9
	SceneManager::GetInstance()->AddGameScene(new ShadowMappingScene());
#endif
#ifdef W10
	SceneManager::GetInstance()->AddGameScene(new ParticleTestScene());
#endif
#ifdef final
	SceneManager::GetInstance()->AddGameScene(new MenuScene());
	SceneManager::GetInstance()->AddGameScene(new DoodleScene());
#endif
#ifdef shader
	SceneManager::GetInstance()->AddGameScene(new ShaderScene());
#endif
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
