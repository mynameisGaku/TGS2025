#include "Catcher.h"
#include "Component/ColliderCapsule.h"

Catcher::Catcher()
{
	m_Collider = nullptr;
}

Catcher::~Catcher()
{
}

void Catcher::Init(CharaDefine::CharaTag tag)
{
	m_Collider = AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;

	switch (tag)
	{
	case CharaDefine::CharaTag::tRed:
		param.tag = ColDefine::Tag::tCatchRed;
		param.targetTags = { ColDefine::Tag::tBallBlue };
		break;
	case CharaDefine::CharaTag::tBlue:
		param.tag = ColDefine::Tag::tCatchBlue;
		param.targetTags = { ColDefine::Tag::tBallRed };
		break;
	}

	m_Collider->BaseInit(param);
	m_Collider->SetDraw(true);
}

void Catcher::Update()
{
	Object3D::Update();
}

void Catcher::Draw()
{
	Object3D::Draw();

	if (m_Collider == nullptr) return;

	if (m_Collider->IsActive())
	{
		DrawSphere3D(transform->Global().position, transform->Global().scale.x / 2, 4, 0xFF00FF, 0x000000, true);
	}
}

void Catcher::SetColliderActive(bool isActive)
{
	m_Collider->SetIsActive(isActive);
}
