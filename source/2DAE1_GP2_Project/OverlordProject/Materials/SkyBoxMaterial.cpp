
#include "stdafx.h"

#include "Base/GeneralStructs.h"
#include "Content/ContentManager.h"
#include "SkyBoxMaterial.h"

SkyBoxMaterial::SkyBoxMaterial() :
	Material(L"./Resources/Effects/SkyBox.fx")
{
}


SkyBoxMaterial::~SkyBoxMaterial()
{
}

void SkyBoxMaterial::SetCubeMapTexture(const wstring& assetFile)
{
	if (assetFile != L"")
	{
		m_pTextureData = ContentManager::Load<TextureData>(assetFile);
	}
}

void SkyBoxMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("m_CubeMap");
	m_pSkyboxVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pSkyboxVariable)
		m_pSkyboxVariable->SetResource(m_pTextureData->GetShaderResourceView());

}