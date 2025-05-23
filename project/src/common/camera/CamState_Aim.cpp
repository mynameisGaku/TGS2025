#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ◇汎用
#include "src/util/time/GameTime.h"

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
		targetChara = charaM->TargetChara(m_CharaIndex);
		if (targetChara == nullptr) {
			ChangeState(&Camera::ChaseState);
			return;
		}

		// コーンの範囲に入って居ない場合
		if (not ColFunction::ColCheck_ConeToPoint(cameraCone, targetChara->transform->position).IsCollision()) {
			bool inCone = ColFunction::ColCheck_ConeToPoint(cameraCone, targetChara->transform->position).IsCollision();
			targetChara = nullptr;
			ChangeState(&Camera::ChaseState);
			return;
		}

		const Transform charaTrs = chara->transform->Global();			// 追従キャラのトランスフォーム
		const Transform targetTrs = targetChara->transform->Global();	// 注視キャラのトランスフォーム

		// カメラ座標と追従キャラ座標を一致させる
		transform->position = charaTrs.position;

		// カメラの相対座標を設定
		SetOffset(CAMERADEFINE_REF.m_OffsetAim);

		const Vector3 targetDiff = targetTrs.position + Vector3::SetY(100.0f) - target;	// 注視点の差異

		// カメラの注視点を設定
		SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));

		const Vector3 diffPos = targetTrs.position - transform->Global().position;	// 注視キャラとカメラの座標の差異
		float targetRot = atan2f(diffPos.x, diffPos.z);		// カメラから注視キャラへの角度
		float diffRot = targetRot - transform->rotation.y;	// 現在の向きと注視キャラへの角度の差異
		const float ROT_X = MathUtil::ToRadians(-20.0f);			// カメラのX軸回転値

		// 回転角度を適応
		MathUtil::RotLimit(&diffRot);
		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;
		MathUtil::RotLimit(&transform->rotation.y);

		// カメラとキャラの向きを揃える
		chara->transform->rotation.y = transform->rotation.y;

		//ColCheckToTerrain();

		if (not InputManager::Hold(KeyCode::RightClick))
		{
			targetChara = nullptr;
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
		targetChara = nullptr;
		canMove = true;
	}
	break;
	}
}