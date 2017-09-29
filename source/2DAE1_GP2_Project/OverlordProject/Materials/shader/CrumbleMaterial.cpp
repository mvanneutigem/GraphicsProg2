//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CrumbleMaterial.h"
#include "Base/GeneralStructs.h"
#include "Content/ContentManager.h"


CrumbleMaterial::CrumbleMaterial() :
	Material(L"./Resources/Effects/CrumbleShader.fx", L"")
{
}


CrumbleMaterial::~CrumbleMaterial()
{
}

void CrumbleMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void CrumbleMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	if (assetFile != L"")
	{
		m_pTextureData = ContentManager::Load<TextureData>(assetFile);
	}
}


void CrumbleMaterial::PauseTime(bool pause)
{
	m_Pause = pause;
}

void CrumbleMaterial::SetTime(float time)
{
	m_Timer = time;
}

void CrumbleMaterial::SetFallSeedY(float fallseed)
{
	m_FallSeedY = fallseed;
}

void CrumbleMaterial::SetFallMultiplier(float fallmultiply)
{
	m_FallMultiply = fallmultiply;
}

void CrumbleMaterial::SetThickness(float thickness)
{
	m_Thickness = thickness;
}

void CrumbleMaterial::SetGridSize(float gridsize)
{
	m_VoxelGridSize = gridsize;
}

void CrumbleMaterial::SetBlockSize(float blocksize)
{
	m_VoxelBlockSize = blocksize;
}

void CrumbleMaterial::SetFallSeedX(float fallseed)
{
	m_FallSeedX = fallseed;
}


void CrumbleMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gDiffuseMap");
	m_pDiffuseMapVariabele = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gLightDirection");
	m_pLightDirectionVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_Timer");
	m_pTimerVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_FallSeedY");
	m_pFallSeedYVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_FallMultiply");
	m_pFallMultiplyVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_Thickness");
	m_pThicknessVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_VoxelGridSize");
	m_pVoxelGridSizeVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_VoxelBlockSize");
	m_pVoxelBlockSizeVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("m_FallSeedX");
	m_pFallSeedXVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
}

void CrumbleMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (!m_Pause)
		m_Timer += gameContext.pGameTime->GetElapsed();

	if (m_pLightDirectionVariable)
		m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);

	if (m_pDiffuseMapVariabele)
		m_pDiffuseMapVariabele->SetResource(m_pTextureData->GetShaderResourceView());

	if (m_pTimerVariable)
		m_pTimerVariable->SetFloat(m_Timer);

	if (m_pFallSeedYVariable)
		m_pFallSeedYVariable->SetFloat(m_FallSeedY);

	if (m_pFallMultiplyVariable)
		m_pFallMultiplyVariable->SetFloat(m_FallMultiply);

	if (m_pThicknessVariable)
		m_pThicknessVariable->SetFloat(m_Thickness);

	if (m_pVoxelGridSizeVariable)
		m_pVoxelGridSizeVariable->SetFloat(m_VoxelGridSize);

	if (m_pVoxelBlockSizeVariable)
		m_pVoxelBlockSizeVariable->SetFloat(m_VoxelBlockSize);

	if (m_pFallSeedXVariable)
		m_pFallSeedXVariable->SetFloat(m_FallSeedX);
}
