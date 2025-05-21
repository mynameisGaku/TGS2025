#include "Camera.h"
#include "CameraDefine.h"
#include "TinyFSM.h"

// ◇汎用
#include "../Library/time.h"
#include "Util/Utils.h"

// ◇個別で必要な物
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

#include "CharaManager.h"
#include "CameraDefineRef.h"

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
		const CharaBase* targetChara = charaM->TargetChara(m_CharaIndex);
		if (targetChara == nullptr) {
			ChangeState(&Camera::ChaseState);
			return;
		}

		// 追従キャラのトランスフォーム
		const Transform charaTrs = chara->transform->Global();

		// 注視キャラのトランスフォーム
		const Transform targetTrs = targetChara->transform->Global();

		// 注視点の差異
		const Vector3 targetDiff = targetTrs.position + V3::SetY(100.0f) - target;

		transform->position = charaTrs.position;

		// カメラの相対座標を設定
		SetOffset(CAMERADEFINE_REF.m_OffsetAim);

		// カメラの注視点を設定
		SetTarget(target + targetDiff * Vector3(0.1f, 0.25f, 0.1f));

		// 注視キャラとカメラの座標の差異
		const Vector3 diffPos = targetTrs.position - transform->Global().position;

		// カメラから注視キャラへの角度
		float targetRot = atan2f(diffPos.x, diffPos.z);

		// 現在の向きと注視キャラへの角度の差異
		float diffRot = targetRot - transform->rotation.y;

		// カメラのX軸回転値
		const float ROT_X = Math::DegToRad(-20.0f);

		// 回転角度を適応
		Function::RotLimit(&diffRot);
		transform->rotation.x += (ROT_X - transform->rotation.x) * 0.1f;
		transform->rotation.y += diffRot * 0.1f;
		Function::RotLimit(&transform->rotation.y);

		chara->transform->rotation.y = transform->rotation.y;

		//ColCheckToTerrain();

		if (not InputManager::Hold(KeyCode::RightClick))
		{
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
		canMove = true;
	}
	break;
	}
}