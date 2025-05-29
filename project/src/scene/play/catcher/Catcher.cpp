#include "src/scene/play/catcher/Catcher.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include <src/util/fx/effect/EffectManager.h>
#include "src/common/component/physics/Physics.h"

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
	EffectManager::Play3D("Catch_Success.efk", m_Parent->transform->Global() + Vector3(0.0f, 150.0f, 0.0f), "Catch_Success");

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
		m_Parent->CatchSuccess(ball->GetComponent<Physics>()->velocity);
		m_Parent->SetBall(ball);
		m_Parent->GetStatusTracker()->AddCatchCount(1);
	}
}

void Catcher::SetColliderActive(bool isActive)
{
	m_Collider->SetIsActive(isActive);
}

bool Catcher::IsColliderActive() const
{
	return m_Collider->IsActive();
}
