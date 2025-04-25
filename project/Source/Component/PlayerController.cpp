#include "PlayerController.h"

#include "../../Library/time.h"

#include "../InputManager.h"
#include "../PadController.h"
#include "../CameraManager.h"
#include "../CharaBase.h"
#include "../StateManager.h"

#include "Physics.h"

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
		if (chara->IsChargingBall())
		{
			if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Begin))
			{
				chara->GenerateBall();
			}
		}
	}
	else
	{
		if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Begin))
		{
			chara->ThrowBallForward();
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
	Vector3 stick = InputManager::AnalogStick(padNumber) * MGetRotY(camera->transform->rotation.y);

	chara->Move(stick);
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
	bool isTiltStick = (InputManager::AnalogStick(padNumber).Size() >= KeyDefine::STICK_DEADZONE);

	// 移動キーが入力されているか
	bool isPushKey = InputManager::Hold("Movement", padNumber);

	return (isTiltStick || isPushKey);
}
