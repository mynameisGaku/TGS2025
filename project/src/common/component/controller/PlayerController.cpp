#include "src/common/component/controller/PlayerController.h"

#include "src/util/time/GameTime.h"

#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/fsm/StateManager.h"

#include "src/common/component/physics/Physics.h"

PlayerController::PlayerController() {

	chara = nullptr;
	padNumber = -1;
}

PlayerController::~PlayerController() {

}

void PlayerController::Init(int _padNumber) {

	chara = Parent<CharaBase>();

	SetPadNumber(_padNumber);
}

void PlayerController::Update() {

	////////////////////////////////////////////////////
	// ボール投げる処理

	// ToDo : チャージ未対応なので対応させるｒ
	if (not chara->IsHoldingBall())
	{
		if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Begin) or
			IsPressButton(KeyDefine::KeyCode::G, KeyDefine::Begin))
		{
			chara->GenerateBall();
		}
	}
	else
	{
		if (not chara->IsChargingBall())
		{
			if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Begin) or
				IsPressButton(KeyDefine::KeyCode::G, KeyDefine::Begin))
			{
				chara->StartBallCharge();
			}
		}
		else
		{
			if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Ended))
			{
				chara->ThrowBallForward();
			}
			if (IsPressButton(KeyDefine::KeyCode::G, KeyDefine::Ended))
			{
				chara->ThrowHomingBall();
			}
		}
	}

	////////////////////////////////////////////////////
	// 吸引キャッチ処理
	if (IsPressButton(KeyDefine::KeyCode::X, KeyDefine::Stationary))
	{
		chara->Catch();
	}

	////////////////////////////////////////////////////
	// ジャンプ処理
	if (IsPressButton(KeyDefine::KeyCode::Space, KeyDefine::Begin))
	{
		chara->Jump();
	}

	////////////////////////////////////////////////////
	// スライディング処理
	if (IsPressButton(KeyDefine::KeyCode::LeftShift, KeyDefine::Stationary) ||
		IsPressButton(KeyDefine::KeyCode::LeftControl, KeyDefine::Stationary) ||
		IsPressButton(KeyDefine::KeyCode::C, KeyDefine::Stationary))
	{
		chara->Slide();
	}

	////////////////////////////////////////////////////
	// テレポート処理
	if (IsPressButton(KeyDefine::KeyCode::E, KeyDefine::Begin))
	{
		chara->TeleportToLastBall();
	}

	////////////////////////////////////////////////////
	// キャラ移動操作処理
	if (chara == nullptr || IsMoveButton() == false)
		return;

	Camera* camera = CameraManager::MainCamera();
	if (camera == nullptr)
		return;

	// カメラの向きに応じたスティックの傾き
	if (false)
	{
		Vector3 stick = InputManager::AnalogStick(padNumber) * MGetRotY(camera->transform->rotation.y);
		chara->Move(stick);
	}
	else
	{
		Vector3 dir;
        if (IsPressButton(KeyDefine::KeyCode::W, KeyDefine::Stationary))
            dir.z += 1.0f;
        if (IsPressButton(KeyDefine::KeyCode::A, KeyDefine::Stationary))
            dir.x -= 1.0f;
        if (IsPressButton(KeyDefine::KeyCode::S, KeyDefine::Stationary))
            dir.z -= 1.0f;
        if (IsPressButton(KeyDefine::KeyCode::D, KeyDefine::Stationary))
            dir.x += 1.0f;

		chara->Move(dir * MGetRotY(camera->transform->rotation.y));
	}

}

Vector3 PlayerController::AnalogStick() {

	return InputManager::AnalogStick(padNumber);
}

bool PlayerController::IsPressButton(KeyDefine::KeyCode keyCode, KeyDefine::TouchPhase toushPhase) {

	if (CanMove() == false)
		return false;

	bool result = false;

	switch (toushPhase) {
	case KeyDefine::Begin:
		result = InputManager::Push(keyCode, padNumber);
		break;

	case KeyDefine::Moved:
	case KeyDefine::Stationary:
		result = InputManager::Hold(keyCode, padNumber);
		break;

	case KeyDefine::Ended:
		result = InputManager::Release(keyCode, padNumber);
		break;

	default:
		break;
	}

	return result;
}

bool PlayerController::IsPressButton(const std::string& input, KeyDefine::TouchPhase toushPhase) {

	if (CanMove() == false)
		return false;

	bool result = false;

	switch (toushPhase) {
	case KeyDefine::Begin:
		result = InputManager::Push(input, padNumber); 
		break;

	case KeyDefine::Moved:
	case KeyDefine::Stationary:
		result = InputManager::Hold(input, padNumber);
		break;

	case KeyDefine::Ended:
		result = InputManager::Release(input, padNumber);
		break;

	default:
		break;
	}

	return result;
}

bool PlayerController::CanMove() const {

	return true;
}

bool PlayerController::IsMoveButton() const {

	if (IsActive() == false || padNumber < 0 || CanMove() == false)
		return false;

	// スティックが傾いているか
	bool isTiltStick = (InputManager::AnalogStick(padNumber).GetLength() >= KeyDefine::STICK_DEADZONE);

	// 移動キーが入力されているか
	bool isPushKey = InputManager::Hold("Movement", padNumber);

	return (isTiltStick || isPushKey);
}
