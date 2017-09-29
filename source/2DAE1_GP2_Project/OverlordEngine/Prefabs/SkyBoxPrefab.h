#pragma once
#include "../Scenegraph/GameObject.h"

class SkyBoxPrefab :public GameObject
{
public:
	SkyBoxPrefab(const wstring& assetFile);
	~SkyBoxPrefab();

protected:
	virtual void Initialize(const GameContext& gameContext);

private:
	wstring m_AssetFile;
};

