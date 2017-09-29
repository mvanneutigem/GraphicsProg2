//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "SpriteTextScene.h"
#include "Graphics/TextRenderer.h"
#include "Content/ContentManager.h"
#define FPS_COUNTER 1

SpriteTextScene::SpriteTextScene(void) :
	GameScene(L"SpriteTextScene")
{
}


SpriteTextScene::~SpriteTextScene(void)
{
}

void SpriteTextScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/fonts/consolas32.fnt");
}

void SpriteTextScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SpriteTextScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	TextRenderer::GetInstance()->DrawText(m_pSpriteFont,L"Hello World!", {10,10},XMFLOAT4{Colors::Blue});
}
