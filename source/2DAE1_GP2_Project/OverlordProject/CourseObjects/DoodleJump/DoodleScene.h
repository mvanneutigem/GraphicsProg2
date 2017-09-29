#pragma once
#include "Scenegraph/GameScene.h"
#include "../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/FixedCamera.h"
#include "JumpCharacter.h"
#include "Platform.h"
#include "Graphics/SpriteFont.h"

class Button;
class PickUp;
class ModelComponent;
class Enemy;

class DoodleScene : public GameScene
{
public:
	DoodleScene(void);
	virtual ~DoodleScene(void);
protected:

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;

private:
	JumpCharacter* m_pChar = nullptr;
	ModelComponent* m_pCharModel = nullptr;
	FixedCamera* m_pfixedCamera = nullptr;
	SpriteFont* m_pSpriteFont = nullptr;
	Button* m_pRestart = nullptr;
	PickUp* m_PickUp = nullptr;
	Enemy* m_pEnemy = nullptr;
	Enemy* m_pEnemy2 = nullptr;

	//hud
	TextureData* m_pEye = nullptr;
	TextureData* m_pTargetSprite = nullptr;
	TextureData* m_pPlus1kSprite = nullptr;
	XMFLOAT2 m_1KPos = { 0,0 };
	float m_1kTime = 0;
	const int TARGETDIM = 32;

	float m_PickUpTime = 10.0f;
	float m_EnemyTime = 5.0f;
	float m_Enemy2Time = 30.0f;
	float m_CamSpeed = 20.0f;
	bool m_SpawnPickUp = false;
	bool m_SpawnEnemy = false;
	bool m_SpawnEnemy2 = false;
	bool m_Dead = false;

	int m_Level = 1;
	float m_GameTime = 0;

	std::vector<Platform*> platformArr;
	int m_Height = 0;
	int m_Score = 0;
	int m_HighScore = 0;
	int m_Eyes = 0;

	void Restart(GameContext gameContext);

	//sounds
	FMOD::Sound *m_pSound, *m_pSound2, *m_pSound3, *m_pSound4;
	FMOD::Channel *m_pChannel, *m_pChannel2, *m_pChannel3, *m_pChannel4;
	bool m_Started = false;
	bool m_Paused = false;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DoodleScene(const DoodleScene &obj) = delete;
	DoodleScene& operator=(const DoodleScene& obj) = delete;
};

