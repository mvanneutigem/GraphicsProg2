#include "stdafx.h"
#include "BoneObject.h"
#include "Components/ModelComponent.h"
#include "Components/TransformComponent.h"


BoneObject::BoneObject(int boneId, int materialId, float length):
	m_BoneId(boneId), 
	m_MaterialId(materialId),
	m_Length(length)
{
}

BoneObject::~BoneObject()
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(XMFLOAT3(0, 0, -m_Length));
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{

	auto boneWorld = XMLoadFloat4x4(&GetTransform()->GetWorld());
	auto inverse = DirectX::XMMatrixInverse(nullptr, boneWorld);
	XMStoreFloat4x4(&m_BindPose, inverse);

	auto children = GetChildren<BoneObject>();
	for(auto c : children)
	{
		c->CalculateBindPose();
	}
}

void BoneObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto modelComponent = new ModelComponent(L"Resources/Meshes/Bone.ovm");
	modelComponent->SetMaterial(m_MaterialId);
	auto obj = new GameObject();
	obj->GetTransform()->Scale(m_Length, m_Length, m_Length);
	obj->AddComponent(modelComponent);
	AddChild(obj);

}
