#include "stdafx.h"

#include "DiffuseMaterial.h"
#include "Base/GeneralStructs.h"
#include "Content/ContentManager.h"

DiffuseMaterial::DiffuseMaterial() : 
Material(L"./Resources/Effects/PosNormTex3D.fx")
{
}


DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile)
{	
	if (assetFile != L"")
	{
		m_pTextureData = ContentManager::Load<TextureData>(assetFile);
	}
}

void DiffuseMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gDiffuseMap");
	m_pDiffuseMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseMapVariabele)
		m_pDiffuseMapVariabele->SetResource(m_pTextureData->GetShaderResourceView());

}