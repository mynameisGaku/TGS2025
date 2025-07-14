#include "src/common/component/controller/PlayerController.h"

#include "src/util/time/GameTime.h"

#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/chara/Chara.h"
#include "src/util/fsm/StateManager.h"
#include "src/util/math/Vector3.h"
#include "src/common/component/physics/Physics.h"
#include <src/reference/network/NetworkRef.h>

PlayerController::PlayerController() {

	chara = nullptr;
	padNumber = 1;
}

PlayerController::~PlayerController() {

}

void PlayerController::Init(int _padNumber) {

	chara = Parent<Chara>();

	SetPadNumber(_padNumber);
}

void PlayerController::Update() {

	////////////////////////////////////////////////////
	// ボール投げる処理

	// 生成禁止
	//if (not chara->IsHoldingBall())
	//{
	//	/*if (InputManager::Push("Throw", padNumber))
	//	{
	//		chara->GenerateBall();
	//	}*/
	//}
	//else
	
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (not chara->GetUser().IsHost)
			return;
	}

	if(chara->CanThrow() && chara->IsHoldingBall())
	{
		if (not chara->IsChargingBall())
		{
			if (InputManager::Push("Throw", padNumber))
			{
				chara->StartBallCharge();
			}
		}
		else
		{
			if (not InputManager::Hold("Throw", padNumber))
			{
				chara->StartThrow();
			}
			else if (InputManager::Push("Feint", padNumber))
			{
				chara->Feint();
			}
		}
	}

	////////////////////////////////////////////////////
	// 吸引キャッチ処理
	if (InputManager::Hold("Catch", padNumber))
	{
		if (chara->CanCatch())
			chara->Catch();
	}

	////////////////////////////////////////////////////
	// ジャンプ処理
	if (InputManager::Push("Jump", padNumber))
	{
		//if (chara->CanMove() && not chara->IsJumping())
			chara->Jump();
			chara->WallAction();
	}

	////////////////////////////////////////////////////
	// スライディング処理
	if (InputManager::Hold("Slide", padNumber))
	{
		chara->Slide();
	}

	////////////////////////////////////////////////////
	// テレポート処理
	if (InputManager::Push("Teleport", padNumber))
	{
		chara->TeleportToLastBall();
	}

	////////////////////////////////////////////////////
	// キャラ移動操作処理
	Camera* camera = CameraManager::GetCamera(chara->GetIndex());
	if (camera == nullptr)
		return;

	// カメラの向きに応じたスティックの傾き
	Vector3 stick = InputManager::AnalogStick(padNumber) * MGetRotY(camera->transform->rotation.y);
	if (stick.GetLengthSquared() > 0.0f)
	{
		chara->Move(stick);
	}

	////////////////////////////////////////////////////
	// タックル
	if (chara->CanTackle() && chara->IsFinishTackleIntervalAlarm())
	{
		if (InputManager::Push("Tackle", padNumber))
		{
			chara->Tackle();
		}
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
