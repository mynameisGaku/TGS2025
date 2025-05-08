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
		OperationByMouse();
		OperationByStick();

		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
		if (chara == nullptr)
			return;

		chara->transform->rotation.y = transform->rotation.y;

		const Transform charaTrs = chara->transform->Global();

		SetOffset(CAMERA_OFFSET_AIM);
		SetTarget(Vector3(50.0f, 100.0f, 100.0f) * charaTrs.Matrix());
		transform->position = charaTrs.position;

		ColCheckToTerrain();

		transform->rotation.x = Math::Clamp(transform->rotation.x, Math::DegToRad(-135.0f), Math::DegToRad(20.0f));

		if (transform->rotation.y < -Math::PI)
			transform->rotation.y += Math::PI_TW;
		else if (transform->rotation.y > Math::PI)
			transform->rotation.y -= Math::PI_TW;

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