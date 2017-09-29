#pragma once
#include <Scenegraph/GameScene.h>

class CharacterTest : public GameScene
{
public:
	CharacterTest();
	~CharacterTest();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:

	GameObject* m_pChar = nullptr;

private:
	CharacterTest(const CharacterTest &obj) = delete;
	CharacterTest& operator=(const CharacterTest& obj) = delete;
};

