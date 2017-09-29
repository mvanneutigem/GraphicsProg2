#pragma once
#include "Graphics\Material.h"
#include "Graphics/TextureData.h"

class DiffuseMaterial_Shadow :public Material
{
public:
	DiffuseMaterial_Shadow();
	~DiffuseMaterial_Shadow();

	void SetDiffuseTexture(const wstring& assetFile);
	void SetLightDirection(XMFLOAT3 lightDir);
	void SetWorld(XMFLOAT4X4 world);

	void UnbindShadowMap(const GameContext& gameContext) override
	{
		if (m_pTechnique)
		{
			m_pEffect->GetVariableByName("shadowMap")->AsShaderResource()->SetResource(0);
			m_pTechnique->GetPassByIndex(0)->Apply(0, gameContext.pDeviceContext);
		}
	}
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DiffuseMaterial_Shadow(const DiffuseMaterial_Shadow &obj);
	DiffuseMaterial_Shadow& operator=(const DiffuseMaterial_Shadow& obj);

	TextureData* m_pTextureData = nullptr;
	XMFLOAT4X4 m_world;
	XMFLOAT3 m_light;
	ID3DX11EffectShaderResourceVariable   *m_pDiffuseMapVariabele = nullptr;
	ID3DX11EffectShaderResourceVariable   *m_ShadowMapVariabele = nullptr;
	ID3DX11EffectMatrixVariable* m_pLightVPMatrixVariable = nullptr;
	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable = nullptr;
};
