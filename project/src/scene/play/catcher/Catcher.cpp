#include "src/scene/play/catcher/Catcher.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include "src/util/fx/effect/EffectManager.h"
#include "src/common/component/physics/Physics.h"
#include "src/util/input/PadController.h"
#include "src/util/math/Vector3Util.h"

// カメラ
#include "src/common/camera/CameraManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/shake/Shake.h"

// 力場
#include "src/scene/play/force_field/ForceFieldManager.h"
#include "src/scene/play/force_field/ForceFieldCorn.h"
#include "src/scene/play/force_field/ConstantPointForce.h"

namespace
{
	static const float ARM_HEIGHT = 150.0f;
}

Catcher::Catcher()
{
	m_pCollider = nullptr;
	m_pParent = nullptr;
	m_pForceFieldManager = nullptr;
	m_pWindArea = nullptr;
}

Catcher::~Catcher()
{
}

void Catcher::Init(std::string tag)
{
	m_pCollider = AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;

	if (tag == "Red")
	{
		param.tag = ColDefine::Tag::tCatch;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tBlue };
	}
	else if (tag == "Blue")
	{
		param.tag = ColDefine::Tag::tCatch;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tBlue };
	}
	else
	{
		param.tag = ColDefine::Tag::tCatch;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tBlue };
	}

	m_pCollider->BaseInit(param);

	m_pForceFieldManager = FindGameObject<ForceFieldManager>();
}

void Catcher::Update()
{
	Object3D::Update();

	if (m_pWindArea)
	{
		Camera* camera = CameraManager::GetCamera(m_pParent->GetIndex());
		Vector3 rot;
		if (camera == nullptr)
		{
			rot = m_pParent->transform->rotation;
		}
		else
		{
			rot = Vector3Util::DirToEuler(camera->TargetLay().Normalize());
		}

		m_pWindArea->transform->position = m_pParent->transform->position + Vector3::SetY(ARM_HEIGHT);
		m_pWindArea->transform->rotation = rot;
	}
}

void Catcher::Draw()
{
	Object3D::Draw();/*

	Camera* camera = CameraManager::GetCamera(m_pParent->GetIndex());
	Vector3 forward;
	if (camera == nullptr)
	{
		forward = m_pParent->transform->Forward();
	}
	else
	{
		forward = camera->TargetLay();
	}
	DrawCone3D(transform->position, transform->position + forward * 100.0f, 30.0f, 8, 0x00FFFF, 0x000000, true);*/
}

void Catcher::CollisionEvent(const CollisionData& colData)
{
	Ball* ball = colData.Other()->Parent<Ball>();

	if (ball->GetState() == Ball::S_OWNED)
		return;

	bool isCatch = false;
	if (m_pParent->GetCharaTag() != ball->GetCharaTag())
	{
		isCatch = true;
	}
	else if (ball->GetState() == Ball::S_LANDED)
	{
		isCatch = true;
	}

	if (not isCatch) return;
	if (not CanCatch(ball)) return;

	//=== キャッチ成功処理 ===

	CatchSuccese(ball);
}

void Catcher::SetColliderActive(bool isActive)
{
	if (IsColliderActive() != isActive)
	{
		if (isActive)
		{
			createWindArea();
		}
		else
		{
			deleteWindArea();
		}
	}

	m_pCollider->SetIsActive(isActive);
}

bool Catcher::IsColliderActive() const
{
	return m_pCollider->IsActive();
}

bool Catcher::CanCatch(Ball* ball) const
{
	static const float CATCH_ANGLE_THRESHOLD = MathUtil::ToRadians(45.0f); // キャッチ可能な角度の閾値

	Camera* camera = CameraManager::GetCamera(m_pParent->GetIndex());
	Vector3 forward;
	if (camera == nullptr)
	{
		forward = m_pParent->transform->Forward();
	}
	else
	{
		forward = camera->TargetLay().Normalize();
	}

	const Vector3 ballLastPosition = ball->transform->position - ball->GetComponent<Physics>()->velocity;
	const Vector3 toBall = Vector3::Normalize(ballLastPosition - m_pParent->transform->position);

	if (Vector3Util::AngleBetween(forward, toBall) > CATCH_ANGLE_THRESHOLD)
	{
		// キャッチ可能な角度ではない
		return false;
	}

	return true;
}

void Catcher::CatchSuccese(Ball* ball) {

	if (ball == nullptr)
		return;

	EffectManager::Play3D("Catch_Success.efk", m_pParent->transform->Global() + Vector3(0.0f, 150.0f, 0.0f), "Catch_Success");

	float charge = ball->GetChargeRate();

	m_pParent->CatchSuccess(ball->GetComponent<Physics>()->velocity);
	m_pParent->SetBall(ball);
	ball->SetOwner(m_pParent);
	ball->PickUp();
	ball->SetChargeRate(charge);
	m_pParent->GetStatusTracker()->AddCatchCount(1);

	Camera* camera = CameraManager::GetCamera(m_pParent->GetIndex());

	if (camera != nullptr) {

		// カメラ振動処理
		Shake* cameraShake = camera->GetComponent<Shake>();
		if (cameraShake != nullptr)
			cameraShake->SetParam({ Shake::Type::tHorizontal, Shake::Type::tDepth }, 5.0f, Vector3(0.15f), 0.5f);

		CameraDefine::CameraAnimData cameraAnimData;

		cameraAnimData.SetAnim(CAMERADEFINE_REF.m_OffsetChase, CAMERADEFINE_REF.m_OffsetChase * 0.6f, 0.2f, 0.5f);
		cameraAnimData.SetRotMat(Vector3::Zero, Vector3(0.0f, 0.0f, MathUtil::ToRadians(-8.0f)), 0.2f, 0.5f);
		cameraAnimData.SetHoldTime(0.35f);

		// カメラの位置を調整
		camera->SetAnimation(cameraAnimData);
	}

	PadController::SetVibration(m_pParent->GetIndex() + 1, 250, 4.0f);
}

void Catcher::createWindArea()
{
	if (not m_pForceFieldManager)
		return;

	m_pWindArea = m_pForceFieldManager->CreateForceFieldCorn(Transform(m_pParent->transform->position + Vector3::SetY(ARM_HEIGHT)), 2000.0f, MathUtil::ToRadians(30.0f));
	m_pWindArea->SetColTag(ColDefine::Tag::tWindArea);
	m_pWindArea->SetColTargetTags({ ColDefine::Tag::tBall, ColDefine::Tag::tChara });

	std::unique_ptr<ConstantPointForce> force = std::make_unique<ConstantPointForce>(-1000.0f);
	m_pWindArea->SetForce(std::move(force));
}

void Catcher::deleteWindArea()
{
	if (not m_pForceFieldManager)
		return;

	m_pForceFieldManager->DeleteForceField(m_pWindArea);
	m_pWindArea = nullptr;
}
