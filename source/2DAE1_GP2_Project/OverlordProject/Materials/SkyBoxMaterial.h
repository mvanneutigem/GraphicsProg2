#pragma once
#include "../../OverlordEngine/Graphics/Material.h"
#include "../../OverlordEngine/Graphics/TextureData.h"

class SkyBoxMaterial :public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();

	void SetCubeMapTexture(const wstring& assetFile);
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxMaterial(const SkyBoxMaterial &obj);
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj);

	TextureData* m_pTextureData = nullptr;
	ID3DX11EffectShaderResourceVariable   *m_pSkyboxVariable = nullptr;

};

