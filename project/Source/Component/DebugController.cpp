#include "DebugController.h"

#include "../../Library/time.h"

#include "../InputManager.h"
#include "../PadController.h"
#include "../CameraManager.h"
#include "../CharaBase.h"
#include "../StateManager.h"

#include "Physics.h"

DebugController::DebugController() 
{

	chara = nullptr;
	padNumber = -1;
}

DebugController::~DebugController() 
{

}

void DebugController::Init(int _padNumber) 
{
	chara = Parent<CharaBase>();

	SetPadNumber(_padNumber);
}

void DebugController::Update() 
{

	////////////////////////////////////////////////////
	// ボール投げる処理
	if (PadController::NormalizedRightTrigger() > KeyDefine::TRIGGER_DEADZONE)
	{
		if (not isPush)
		{
			//chara->GenerateBall();
			//chara->ThrowHomingBall();
			isPush = true;
		}
	}
	else isPush = false;

	////////////////////////////////////////////////////
	// 吸引キャッチ処理
	if (PadController::NormalizedLeftTrigger() > KeyDefine::TRIGGER_DEADZONE)
	{
		//chara->Catch();
	}

	////////////////////////////////////////////////////
	// ジャンプ処理
	if (IsPressButton(KeyDefine::KeyCode::ButtonA, KeyDefine::Begin))
	{
		chara->Jump();
	}

	////////////////////////////////////////////////////
	// スライディング処理
	if (IsPressButton(KeyDefine::KeyCode::RightThumb, KeyDefine::Stationary))
	{
		//chara->Slide();
	}

	////////////////////////////////////////////////////
	// キャラ移動操作処理
	if (chara == nullptr || IsMoveButton() == false)
		return;

	Camera* camera = CameraManager::MainCamera();
	if (camera == nullptr)
		return;

	// カメラの向きに応じたスティックの傾き
    Vector2 leftStick = PadController::NormalizedLeftStick(padNumber);
	Vector3 virtualStick = Vector3(leftStick.x, 0, leftStick.y);
	Vector3 stick = virtualStick * MGetRotY(camera->transform->rotation.y);

	chara->Move(stick);
}

bool DebugController::IsPressButton(KeyDefine::KeyCode keyCode, KeyDefine::TouchPhase toushPhase) 
{

	if (CanMove() == false)
		return false;

	bool result = false;

	switch (toushPhase) 
	{
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

bool DebugController::IsPressButton(const std::string& input, KeyDefine::TouchPhase toushPhase) 
{

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

bool DebugController::CanMove() const 
{

	return true;
}

bool DebugController::IsMoveButton() const 
{

	if (IsActive() == false || padNumber < 0 || CanMove() == false)
		return false;

	// スティックが傾いているか
	bool isTiltStick = (PadController::NormalizedLeftStick(padNumber).Size() >= KeyDefine::STICK_DEADZONE);

	return (isTiltStick);
}
