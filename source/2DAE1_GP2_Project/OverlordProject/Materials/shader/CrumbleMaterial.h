#pragma once
#include "Graphics/Material.h"

class CrumbleMaterial: public Material
{
public:
	CrumbleMaterial();
	~CrumbleMaterial();

	void SetLightDirection(XMFLOAT3 direction);
	void SetDiffuseTexture(const wstring& assetFile);
	void PauseTime(bool pause);
	void SetTime(float time);
	void SetFallSeedY(float fallseed);
	void SetFallMultiplier(float fallmultiply);
	void SetThickness(float thickness);
	void SetGridSize(float gridsize);
	void SetBlockSize(float blocksize);
	void SetFallSeedX(float fallseed);

	//getters
	float GetFallSeedY( ) const
	{
		return m_FallSeedY;
	}
	float GetFallMultiplier( ) const
	{
		return m_FallMultiply;
	}
	float GetThickness( ) const
	{
		return m_Thickness;
	}
	float GetGridSize( ) const
	{
		return m_VoxelGridSize;
	}
	float GetBlockSize( ) const
	{
		return m_VoxelBlockSize;
	}
	float GetFallSeedX( ) const
	{
		return m_FallSeedX;
	}

protected:


	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CrumbleMaterial(const CrumbleMaterial &obj);
	CrumbleMaterial& operator=(const CrumbleMaterial& obj);

	XMFLOAT3 m_LightDirection = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	ID3DX11EffectVectorVariable* m_pLightDirectionVariable = nullptr;

	TextureData* m_pTextureData = nullptr;
	ID3DX11EffectShaderResourceVariable   *m_pDiffuseMapVariabele = nullptr;

	float m_Timer = 0.0f;
	bool m_Pause = true;
	ID3DX11EffectScalarVariable* m_pTimerVariable = nullptr;

	float m_FallSeedY = 10;
	ID3DX11EffectScalarVariable* m_pFallSeedYVariable = nullptr;

	float m_FallMultiply = 10.f;
	ID3DX11EffectScalarVariable* m_pFallMultiplyVariable = nullptr;

	float m_Thickness = 0.1f;
	ID3DX11EffectScalarVariable* m_pThicknessVariable = nullptr;

	float m_VoxelGridSize = 3.0f;
	ID3DX11EffectScalarVariable* m_pVoxelGridSizeVariable = nullptr;

	float m_VoxelBlockSize = 0.5f;
	ID3DX11EffectScalarVariable* m_pVoxelBlockSizeVariable = nullptr;

	float m_FallSeedX = 10.f;
	ID3DX11EffectScalarVariable* m_pFallSeedXVariable = nullptr;
};

