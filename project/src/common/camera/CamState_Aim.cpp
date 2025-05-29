#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtil.h"

// ◇個別で必要な物
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/collider/CollisionFunc.h"

using namespace KeyDefine;
using namespace CameraDefine;

void Camera::AimState(FSMSignal sig)
{
	// 移動可能か
	static bool canMove;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
		canMove = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		// キャラクターの管理者
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// 追従するキャラ
		const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
		if (chara == nullptr)
			return;

		// 注視するキャラ
		m_TargetChara = charaM->TargetChara(m_CharaIndex);
		if (m_TargetChara == nullptr || MouseController::Info().Move().GetLengthSquared() > 5.0f) {
			m_TargetTransitionTime = 0.5f;
			ChangeState(&Camera::ChaseState);
			return;
		}

		// コーンの範囲に入って居ない場合
		if (not ColFunction::ColCheck_ConeToPoint(cameraCone, m_TargetChara->transform->position).IsCollision()) {
			m_TargetChara = nullptr;
			ChangeState(&Camera::ChaseState);
			return;
		}

		const Transform charaTrs = chara->transform->Global();			// 追従キャラのトランスフォーム
		const Transform targetTrs = m_TargetChara->transform->Global();	// 注視キャラのトランスフォーム
		Vector3 targetOffset = Vector3::SetY(100.0f);					// 注視点の相対座標
		Vector3 targetPos = targetTrs.position + targetOffset;

		// カメラ座標と追従するキャラの座標を一致させる
		transform->position = charaTrs.position;

		// カメラの相対座標を設定
		SetOffsetAfter(CAMERADEFINE_REF.m_OffsetAim);

		// カメラの注視点を設定
		SetTargetAfter(targetPos);

		const float TARGET_DIR = targetPos.Direction(transform->Global().position);	// カメラから注視キャラへの角度
		const float ROT_X = MathUtil::ToRadians(-20.0f);			// カメラのX軸回転値
		float diffRot = TARGET_DIR - transform->rotation.y;	// 現在の向きと注視キャラへの角度の差異

		// 回転角度に制限をかける
		MathUtil::RotLimit(&diffRot);

		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;

		// 回転角度に制限をかける
		MathUtil::RotLimit(&transform->rotation.y);

		// カメラとキャラの向きを揃える
		//chara->transform->rotation.y = transform->rotation.y;

		if (not InputManager::Hold("TargetCamera"))
		{
			m_TargetChara = nullptr;
			ChangeState(&Camera::ChaseState);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{

	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
		m_TargetChara = nullptr;
		canMove = true;
	}
	break;
	}
}