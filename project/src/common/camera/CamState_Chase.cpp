#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraDefine.h"
#include "src/util/fsm/TinyFSM.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/math/MathUtils.h"

// ◇個別で必要な物
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

#include "src/scene/play/chara/CharaManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/component/collider/CollisionFunc.h"

using namespace KeyDefine;
using namespace CameraDefine;

void Camera::ChaseState(FSMSignal sig)
{
	// 移動可能か
	static bool canMove;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
		canMove = true;

		m_TargetTransitionTime = 0.0f;
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		if (m_TargetTransitionTime > 0.0f)
			m_TargetTransitionTime -= GTime.DeltaTime();
		else
			m_TargetTransitionTime = 0.0f;

		OperationByMouse();
		OperationByStick(m_CharaIndex + 1);

		// キャラクターの管理者
		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		// 追従するキャラクター
		const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
		if (chara == nullptr)
			return;

		// キャラクターのトランスフォーム
		const Transform charaTrs = chara->transform->Global();

		// カメラの相対座標を設定
		SetOffsetAfter(CAMERADEFINE_REF.m_OffsetChase);

		// カメラの注視点を設定
		SetTargetAfter(CAMERADEFINE_REF.m_TargetChase * charaTrs.Matrix());
		
		// カメラ座標と追従するキャラの座標を一致させる
		transform->position = charaTrs.position;

		MathUtil::ClampAssing(&transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min, CAMERADEFINE_REF.m_RotX_Max);
		MathUtil::RotLimit(&transform->rotation.y);

		// 注視するキャラ
		m_TargetChara = charaM->TargetChara(m_CharaIndex);

		// 注視するキャラが存在、ボタン入力がされた場合
		if (m_TargetChara != nullptr && InputManager::Hold("TargetCamera"))
		{
			// マウスの移動検知
			if (MouseController::Info().Move().GetLengthSquared() > 5.0f)
				m_TargetTransitionTime = 0.5f;

			// コーン形状の判定内に注視するキャラが居る場合
			if (m_TargetTransitionTime <= 0.0f && ColFunction::ColCheck_ConeToPoint(cameraCone, m_TargetChara->transform->position).IsCollision())
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