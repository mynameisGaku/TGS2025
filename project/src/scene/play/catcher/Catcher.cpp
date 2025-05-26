#include "src/scene/play/catcher/Catcher.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/scene/play/ball/Ball.h"

Catcher::Catcher()
{
	m_Collider = nullptr;
	m_Parent = nullptr;
}

Catcher::~Catcher()
{
}

void Catcher::Init(std::string tag)
{
	m_Collider = AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;

	if (tag == "Red")
	{
		param.tag = ColDefine::Tag::tCatchRed;
		param.targetTags = { ColDefine::Tag::tBallRed, ColDefine::Tag::tBallBlue };
	}
	else if (tag == "Blue")
	{
		param.tag = ColDefine::Tag::tCatchBlue;
		param.targetTags = { ColDefine::Tag::tBallRed, ColDefine::Tag::tBallBlue };
	}
	else
	{
		param.tag = ColDefine::Tag::tCatchRed;
		param.targetTags = { ColDefine::Tag::tBallRed, ColDefine::Tag::tBallBlue };
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
}

void Catcher::CollisionEvent(const CollisionData& colData)
{
	Ball* ball = colData.Other()->Parent<Ball>();

	if (ball->GetState() == Ball::S_OWNED) return;

	bool isCatch = false;
	if (m_Parent->GetCharaTag() != ball->GetCharaTag())
	{
		isCatch = true;
	}
	else if (ball->GetState() == Ball::S_LANDED)
	{
		isCatch = true;
	}

	if (isCatch)
	{
		m_Parent->SetBall(ball);
	}
}

void Catcher::SetColliderActive(bool isActive)
{
	m_Collider->SetIsActive(isActive);
}
