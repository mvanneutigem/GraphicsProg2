#pragma once
#include "Graphics/Material.h"
#include "Graphics/TextureData.h"

class DiffuseMaterial :public Material
{
public:
	DiffuseMaterial();
	~DiffuseMaterial();

	void SetDiffuseTexture(const wstring& assetFile);
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DiffuseMaterial(const DiffuseMaterial &obj);
	DiffuseMaterial& operator=(const DiffuseMaterial& obj);

	TextureData* m_pTextureData = nullptr;
	ID3DX11EffectShaderResourceVariable   *m_pDiffuseMapVariabele = nullptr;

};

