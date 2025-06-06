#include "src/scene/play/catcher/Catcher.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include <src/util/fx/effect/EffectManager.h>
#include "src/common/component/physics/Physics.h"

#include "src/common/camera/CameraManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/shake/Shake.h"
#include "src/util/input/PadController.h"

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

	if (ball->GetState() == Ball::S_OWNED)
		return;

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
		EffectManager::Play3D("Catch_Success.efk", m_Parent->transform->Global() + Vector3(0.0f, 150.0f, 0.0f), "Catch_Success");

		m_Parent->CatchSuccess(ball->GetComponent<Physics>()->velocity);
		m_Parent->SetBall(ball);
		ball->SetOwner(m_Parent);
		ball->PickUp();
		ball->SetChargeRate(0.1f);
		m_Parent->GetStatusTracker()->AddCatchCount(1);

		Camera* camera = CameraManager::GetCamera(m_Parent->GetIndex());

		if (camera != nullptr) {

			// ƒJƒƒ‰U“®ˆ—
			Shake* cameraShake = camera->GetComponent<Shake>();
			if (cameraShake != nullptr)
				cameraShake->SetParam({ Shake::Type::tHorizontal, Shake::Type::tDepth }, 5.0f, Vector3(0.15f), 0.5f);

			CameraDefine::CameraAnimData cameraAnimData;

			cameraAnimData.SetAnim(CAMERADEFINE_REF.m_OffsetChase, CAMERADEFINE_REF.m_OffsetChase * 0.6f, 0.2f, 0.5f);
			cameraAnimData.SetRotMat(Vector3::Zero, Vector3(0.0f, 0.0f, MathUtil::ToRadians(-8.0f)), 0.2f, 0.5f);
			cameraAnimData.SetHoldTime(0.35f);

			// ƒJƒƒ‰‚ÌˆÊ’u‚ð’²®
			camera->SetAnimation(cameraAnimData);
		}

		PadController::SetVibration(m_Parent->GetIndex() + 1, 250, 4.0f);
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
