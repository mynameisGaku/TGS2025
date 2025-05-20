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

	if (IsPressButton(KeyDefine::KeyCode::Space, KeyDefine::Begin))
	{
		chara->Jump();
	}
	if (IsPressButton(KeyDefine::KeyCode::Z, KeyDefine::Begin))
	{
		chara->Attack();
	}
	if (IsPressButton(KeyDefine::KeyCode::C, KeyDefine::Stationary))
	{
		chara->Block();
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
	bool isTiltStick = (InputManager::AnalogStick(padNumber).Size() >= KeyDefine::STICK_DEADZONE);

	// 移動キーが入力されているか
	bool isPushKey = InputManager::Hold("Movement", padNumber);

	return (isTiltStick || isPushKey);
}
