#pragma once
#include "BaseComponent.h"
#include "../Helpers/VertexHelper.h"
#include "MeshDrawComponent.h"

class TextureData;

class ClothComponent : public BaseComponent
{
public:
	ClothComponent(const PxTransform &pose, int resolution, int sizeX, int sizeY, const wstring& textureFile);
	~ClothComponent();

	void SetWindVec(PxVec3 dir)
	{
		m_pCloth->setExternalAcceleration(dir);
	}

protected:

	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);
	virtual void Initialize(const GameContext& gameContext);

private:
	void ClothComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c);
	void ClothComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d);

	int m_Resolution, m_SizeX, m_SizeY;
	PxTransform m_pose;
	PxCloth* m_pCloth = nullptr;
	vector<PxVec2> m_UVArr;

	PxReal m_Time = 0.0f;

	const wstring m_TextureFile;

	vector<VertexPosNormTex> m_VecVertices;
	vector<DWORD> m_VecIndices;
	PxClothParticle* particles = nullptr;
	PxU32* triangles = nullptr;

	//Rendering
	TextureData* m_pTextureData;
	ID3D11Buffer*m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	unsigned int m_numTriangles = 0;

	MeshDrawComponent* m_pMeshDrawer = nullptr;

	Material *m_pMaterial;
	UINT m_MaterialId;
	bool m_MaterialSet;

	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3DX11Effect* m_pEffect = nullptr;
	ID3DX11EffectTechnique* m_pTechnique = nullptr;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable = nullptr;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable = nullptr;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseVariable = nullptr;

	PxClothCollisionSphere   gSpheres[3] = {
		PxClothCollisionSphere(PxVec3(0.0f, -2.0f, 0.0f), 0.5f),
		PxClothCollisionSphere(PxVec3(0.0f, 2.0f, 0.0f), 0.5f),
		PxClothCollisionSphere(PxVec3(-1.5f, 0.2f, 0.0f), 0.2f)
	};

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ClothComponent(const ClothComponent& yRef);
	ClothComponent& operator=(const ClothComponent& yRef);

};

