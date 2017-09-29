//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "UberMaterial.h"
#include "Base/GeneralStructs.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"

//statics
ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable;

UberMaterial::UberMaterial() :
	Material(L"./Resources/Effects/UberShader.fx"),
	m_LightDirection{.577f,.577f,.577f},
	m_bDiffuseTexture(false),
	m_pDiffuseTexture{},
	m_ColorDiffuse{ 1,1,1,1 },
	m_ColorSpecular{1,1,1,1},
	m_bSpecularLevelTexture(false),
	m_pSpecularLevelTexture{},
	m_Shininess(15),
	m_ColorAmbient(0,0,0,1),
	m_AmbientIntensity(0),
	m_bFlipGreenChannel(false),
	m_bNormalMapping(false),
	m_pNormalMappingTexture{},
	m_bEnvironmentMapping(false),
	m_pEnvironmentCube{},
	m_ReflectionStrength(1),
	m_RefractionStrength(0.1f),
	m_RefractionIndex(0.3f),
	m_Opacity(1),
	m_bOpacityMap(false),
	m_bSpecularBlinn(false),
	m_bSpecularPhong(false),
	m_bFresnelFaloff(false),
	m_ColorFresnel(1,1,1,1),
	m_FresnelPower(1),
	m_FresnelMultiplier(1),
	m_FresnelHardness(1)
{
	
}
UberMaterial::~UberMaterial()
{
	
}

//LIGHT
//*****
void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

//DIFFUSE
//*******
void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}
void UberMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	if (m_bDiffuseTexture)
		m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

//SPECULAR
//********
void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}
void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}
void UberMaterial::SetSpecularLevelTexture(const wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

//AMBIENT
//*******
void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}
void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

//NORMAL MAPPING
//**************
void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}
void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}
void UberMaterial::SetNormalMapTexture(const wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

//ENVIRONMENT MAPPING
//*******************
void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}
void UberMaterial::SetEnvironmentCube(const wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}
void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}
void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}
void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

//OPACITY
//*******
void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}
void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}
void UberMaterial::SetOpacityTexture(const wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

//SPECULAR MODELS
//***************
void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}
void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

//FRESNEL FALLOFF
//***************
void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}
void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}
void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}
void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}
void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	auto effectVar = m_pEffect->GetVariableByName("gLightDirection");
	m_pLightDirectionVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gUseTextureDiffuse");
	m_pUseDiffuseTextureVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gTextureDiffuse");
	m_pDiffuseSRVvariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gColorDiffuse");
	m_pDiffuseColorVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gColorSpecular");
	m_pSpecularColorVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gUseTextureSpecularIntensity");
	m_pUseSpecularLevelTextureVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gTextureSpecularIntensity");
	m_pSpecularLevelSRVvariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gShininess");
	m_pShininessVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gColorAmbient");
	m_pAmbientColorVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gAmbientIntensity");
	m_pAmbientIntensityVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gFlipGreenChannel");
	m_pFlipGreenChannelVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gUseTextureNormal");
	m_pUseNormalMappingVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gTextureNormal");
	m_pNormalMappingSRVvariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gCubeEnvironment");
	m_pUseEnvironmentMappingVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gUseTextureEnvironment");
	m_pEnvironmentSRVvariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gReflectionStrength");
	m_pReflectionStrengthVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gRefractionStrength");
	m_pRefractionStrengthVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gRefractionIndex");
	m_pRefractionIndexVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gOpacityIntensity");
	m_pOpacityVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gUseTextureOpacity");
	m_pUseOpacityMapVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gTextureOpacity");
	m_pOpacitySRVvariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gUseSpecularBlinn");
	m_pUseBlinnVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gUseSpecularPhong");
	m_pUsePhongVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

	effectVar = m_pEffect->GetVariableByName("gUseFresnelFalloff");
	m_pUseFresnelFalloffVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gColorFresnel");
	m_pFresnelColorVariable = (effectVar->IsValid()) ? effectVar->AsVector() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gFresnelPower");
	m_pFresnelPowerVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gFresnelMultiplier");
	m_pFresnelMultiplierVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gFresnelHardness");
	m_pFresnelHardnessVariable = (effectVar->IsValid()) ? effectVar->AsScalar() : nullptr;

}
void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pLightDirectionVariable)
		m_pLightDirectionVariable->SetFloatVector(&m_LightDirection.x);

	if (m_pUseDiffuseTextureVariable)
		m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	if (m_pDiffuseTexture)
	{
		if (m_pDiffuseSRVvariable)
			m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
	if (m_pDiffuseColorVariable)
		m_pDiffuseColorVariable->SetFloatVector(&m_ColorDiffuse.x);

	if (m_pSpecularColorVariable)
		m_pSpecularColorVariable->SetFloatVector(&m_ColorSpecular.x);
	if (m_pUseSpecularLevelTextureVariable)
		m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	if (m_pSpecularLevelTexture)
	{
		if (m_pSpecularLevelSRVvariable)
			m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	}
	if (m_pShininessVariable)
		m_pShininessVariable->SetInt(m_Shininess);

	if (m_pAmbientColorVariable)
		m_pAmbientColorVariable->SetFloatVector(&m_ColorAmbient.x);
	if (m_pAmbientIntensityVariable)
		m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);

	if (m_pFlipGreenChannelVariable)
		m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	if (m_pUseNormalMappingVariable)
		m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);
	if (m_pNormalMappingTexture)
	{
		if (m_pNormalMappingSRVvariable)
			m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}

	if (m_pUseEnvironmentMappingVariable)
		m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	if (m_pEnvironmentCube)
	{
		if (m_pEnvironmentSRVvariable)
			m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	}
	if (m_pReflectionStrengthVariable)
		m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	if (m_pRefractionStrengthVariable)
		m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	if (m_pRefractionIndexVariable)
		m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);

	if (m_pOpacityVariable)
		m_pOpacityVariable->SetFloat(m_Opacity);
	if (m_pUseOpacityMapVariable)
		m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	if (m_pOpacityMap && m_bOpacityMap)
	{
		if (m_pOpacitySRVvariable)
			m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());

	}
	if (m_pUseBlinnVariable)
		m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	if (m_pUsePhongVariable)
		m_pUsePhongVariable->SetBool(m_bSpecularPhong);

	if (m_pUseFresnelFalloffVariable)
		m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	if (m_pFresnelColorVariable)
		m_pFresnelColorVariable->SetFloatVector(&m_ColorFresnel.x);
	if (m_pFresnelPowerVariable)
		m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	if (m_pFresnelMultiplierVariable)
		m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	if (m_pFresnelHardnessVariable)
		m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
}