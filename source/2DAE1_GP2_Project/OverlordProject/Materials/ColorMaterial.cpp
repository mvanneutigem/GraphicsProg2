//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ColorMaterial.h"
#include "Base/GeneralStructs.h"


ColorMaterial::ColorMaterial(bool enableTransparency) : 
	Material(L"./Resources/Effects/PosNormCol3D.fx", enableTransparency?L"TransparencyTech":L"")
{
}


ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gColor");
	m_pDiffuseColorVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gSetColor");
	m_pUseColor = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

}

void ColorMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	if (m_SetColor)
	{
		if (m_pDiffuseColorVariable)
			m_pDiffuseColorVariable->SetFloatVector(&m_Color.x);
		if (m_pUseColor)
			m_pUseColor->SetBool(m_SetColor);
	}
	
}
