#include "stdafx.h"
#include "SkyBoxPrefab.h"
#include "../Components/Components.h"
#include "../../OverlordProject/Materials/SkyBoxMaterial.h"

SkyBoxPrefab::SkyBoxPrefab(const wstring& assetFile):m_AssetFile(assetFile)
{
}

SkyBoxPrefab::~SkyBoxPrefab()
{
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext)
{
	auto skyboxMaterial = new SkyBoxMaterial();
	skyboxMaterial->SetCubeMapTexture(m_AssetFile);
	auto model = new ModelComponent(L"Resources/Meshes/Box.ovm");
	gameContext.pMaterialManager->AddMaterial(skyboxMaterial, 0);
	model->SetMaterial(0);
	AddComponent(model);
}
