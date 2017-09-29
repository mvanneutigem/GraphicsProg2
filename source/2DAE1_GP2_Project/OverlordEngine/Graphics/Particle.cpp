//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings) :
	m_EmitterSettings(emitterSettings)
{
}


Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	auto dTime = gameContext.pGameTime->GetElapsed();
	//See Lab10_2
	if (!m_IsActive)
		return;

	m_CurrentEnergy -= dTime;
	if (m_CurrentEnergy < 0)
	{
		m_IsActive = false;
		return;
	}

	m_VertexInfo.Position.x += m_EmitterSettings.Velocity.x * dTime;
	m_VertexInfo.Position.y += m_EmitterSettings.Velocity.y * dTime;
	m_VertexInfo.Position.z += m_EmitterSettings.Velocity.z * dTime;

	m_VertexInfo.Color = m_EmitterSettings.Color;

	float particleLifePercent = m_CurrentEnergy /m_TotalEnergy;

	m_VertexInfo.Color.w = particleLifePercent;

	if (m_SizeGrow < 1)
	{
		m_VertexInfo.Size = m_InitSize - m_InitSize * m_SizeGrow * (1 - particleLifePercent);
	}
	if (m_SizeGrow > 1)
	{
		m_VertexInfo.Size = m_InitSize + m_InitSize * m_SizeGrow * (1 - particleLifePercent);
	}
}

void Particle::Init(XMFLOAT3 initPosition)
{
	//See Lab10_2
	m_IsActive = true;

	m_TotalEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_CurrentEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);

	XMFLOAT3 randomDirection = XMFLOAT3(1,0,0);
	auto randomDirectionVector = XMLoadFloat3(&randomDirection);
	auto randomMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	auto directionVector = XMVector3TransformNormal(randomDirectionVector, randomMatrix);

	auto distance = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);

	XMFLOAT3 direction;
	XMStoreFloat3(&direction, directionVector);

	m_VertexInfo.Position.x = initPosition.x + distance * direction.x;
	m_VertexInfo.Position.y = initPosition.y + distance * direction.y;
	m_VertexInfo.Position.z = initPosition.z + distance * direction.z;

	m_VertexInfo.Size = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);

	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
