//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount) :
	m_ParticleCount(particleCount),
	m_AssetFile(assetFile)
{
	//See Lab10_2
	for(int i = 0; i < m_ParticleCount; ++i)
	{
		m_Particles.push_back(new Particle(m_Settings));
	}
}


ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	//See Lab10_2
	for (int i = 0; i < m_ParticleCount; ++i)
	{
		delete m_Particles[i];
	}
	m_Particles.clear();

	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	//See Lab10_2
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	//See Lab10_2
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	
	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);

	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);

	auto effectVar = m_pEffect->GetVariableByName("gWorldViewProj");
	m_pWvpVariable = (effectVar->IsValid()) ? effectVar->AsMatrix() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gViewInverse");
	m_pViewInverseVariable = (effectVar->IsValid()) ? effectVar->AsMatrix() : nullptr;
	effectVar = m_pEffect->GetVariableByName("gParticleTexture");
	m_pTextureVariable = (effectVar->IsValid()) ? effectVar->AsShaderResource() : nullptr;

	
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	//See Lab10_2
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;

	auto hr = gameContext.pDevice->CreateBuffer(&buffDesc, nullptr, &m_pVertexBuffer);
	if (Logger::LogHResult(hr, L"ParticleEmitterComponent::CreateVertexBuffer"))
		return;

}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	//See Lab10_2

	//BUFFER MAPPING CODE [PARTIAL :)]
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;
	float particleInterval = ((m_Settings.MaxEnergy + m_Settings.MinEnergy)/2.0f) / m_ParticleCount;
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;

	for(unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		m_Particles[i]->Update(gameContext);
		if (m_Particles[i]->IsActive())
		{
			pBuffer[m_ActiveParticles] = m_Particles[i]->GetVertexInfo();
			++m_ActiveParticles;
		}
		else if(m_LastParticleInit >= particleInterval)
		{
			m_Particles[i]->Init(this->GetTransform()->GetWorldPosition());
			pBuffer[m_ActiveParticles] = m_Particles[i]->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0;
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);


}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	//See Lab10_2
	if (m_pWvpVariable)
	{
		XMMATRIX wvp = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
		m_pWvpVariable->SetMatrix(reinterpret_cast<const float*>(&(wvp)));
	}

	if (m_pViewInverseVariable)
	{
		auto viewInv = XMLoadFloat4x4(&gameContext.pCamera->GetViewInverse());
		m_pViewInverseVariable->SetMatrix(reinterpret_cast<float*>(&viewInv));
	}

	if (m_pTextureVariable)
		m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		m_pDefaultTechnique->GetPassByIndex(i)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
