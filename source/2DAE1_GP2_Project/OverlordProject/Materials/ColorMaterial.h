#pragma once
#include "Graphics/Material.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();
	void SetColor(XMFLOAT4 color)
	{
		m_Color = color;
		m_SetColor = true;
	}

protected:


	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	XMFLOAT4 m_Color;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);

	bool m_SetColor = false;
	ID3DX11EffectVectorVariable* m_pDiffuseColorVariable = nullptr;
	ID3DX11EffectScalarVariable* m_pUseColor= nullptr;
};

