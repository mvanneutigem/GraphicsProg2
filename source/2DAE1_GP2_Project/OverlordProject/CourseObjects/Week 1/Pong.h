#pragma once
#include <Scenegraph/GameScene.h>
class Pong : public GameScene
{
public:
	Pong();
	~Pong();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	GameObject* m_pBatR = nullptr;
	GameObject* m_pBatL = nullptr;
	float m_RightPos = 0.0f;
	float m_LeftPos = 0.0f;
	GameObject* m_pBall = nullptr;
	bool m_StartGame = false;
	GameObject* m_pTriggerActorLeft, *m_pTriggerActorRight;
	int m_RightScore = 0;
	int m_LeftScore = 0;
private:
	Pong(const Pong& other) = delete;
	Pong& operator=(const Pong& other) = delete;
};
