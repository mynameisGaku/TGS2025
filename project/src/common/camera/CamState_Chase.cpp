#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"
#include "src/util/easing/EasingUtils.h"

// ◇個別で必要な物
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/collider/CollisionFunc.h"
#include "src/common/network/NetworkManager.h"
#include "src/scene/play/ball/BallTarget.h"

using namespace KeyDefine;
using namespace CameraDefine;

void Camera::ChaseState(FSMSignal sig)
{
	// 移動可能か
	static bool canMove;
	static const float EASING_TIME = 0.5f;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
		canMove = true;

		m_TargetTransitionTime = 0.0f;
		m_EasingTime = EASING_TIME;

		m_PositionPrev = transform->Global().position;
		m_OffsetPrev = Offset();
		m_TargetPrev = Target();
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		// カメラを持つキャラを取得
		findFollowerChara();
		if (not m_pFollowerChara) return;

		//▼=== エイムステートから滑らかに視点（オフセット）を戻す処理 ===
		m_TargetTransitionTime = max(m_TargetTransitionTime - GTime.DeltaTime(), 0.0f);
		m_EasingTime = max(m_EasingTime - GTime.DeltaTime(), 0.0f);

		const Transform FOLLOWER_TRS = m_pFollowerChara->transform->Global();

		const Vector3 OFFSET = CAMERADEFINE_REF.m_OffsetChase;
		const Vector3 TARGET = CAMERADEFINE_REF.m_TargetChase * FOLLOWER_TRS.Matrix();
		const Vector3 POSITION = FOLLOWER_TRS.position;

		if (m_EasingTime > 0.0f)
		{
			SetOffset(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_OffsetPrev, OFFSET));
			SetTarget(EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_TargetPrev, TARGET));
			transform->position = EasingFunc::InCubic(m_EasingTime, EASING_TIME, m_PositionPrev, POSITION);
		}
		else
		{
			SetOffset(OFFSET);
			SetTarget(TARGET);
			transform->position = POSITION;
		}
		//=========================================================

		// 入力受付、カメラ回転
		operationByStick(m_CharaIndex + 1, ViewPointShift::All);
		if (m_CharaIndex == 0) // プレイヤー0ならマウス操作も可能
		{
			operationByMouse(ViewPointShift::All);
		}
		
		// X軸回転に制限をかける
		MathUtil::ClampAssing(&transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
		
		// Y軸回転に制限をかける
		MathUtil::RotLimitAssing(&transform->rotation.y);

		// 注視するキャラが存在、ボタン入力がされた場合
		if (m_pBallTarget != nullptr && InputManager::Hold("TargetCamera", m_CharaIndex + 1))
		{
			// 視点移動検知
			if (MouseController::Info().Move().GetLengthSquared() > 5.0f ||
				PadController::NormalizedRightStick(m_CharaIndex + 1).GetLengthSquared() >= KeyDefine::STICK_DEADZONE)
				m_TargetTransitionTime = 0.5f;

			// コーン形状の判定内に注視するキャラが居る場合
			if (m_TargetTransitionTime <= 0.0f && ColFunction::ColCheck_ConeToPoint(m_CameraCone, m_pBallTarget->Position()).IsCollision())
				ChangeState(&Camera::AimState);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{

	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
		canMove = true;
	}
	break;
	}
}
