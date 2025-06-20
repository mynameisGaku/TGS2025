#include "src/common/component/controller/PlayerController.h"

#include "src/util/time/GameTime.h"

#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/fsm/StateManager.h"
#include "src/util/math/Vector3.h"
#include "src/common/component/physics/Physics.h"

PlayerController::PlayerController() {

	chara = nullptr;
	padNumber = 1;
}

PlayerController::~PlayerController() {

}

void PlayerController::Init(int _padNumber) {

	chara = Parent<CharaBase>();

	SetPadNumber(_padNumber);
}

void PlayerController::Update() {

	////////////////////////////////////////////////////
	// �{�[�������鏈��

	// �����֎~
	//if (not chara->IsHoldingBall())
	//{
	//	/*if (InputManager::Push("Throw", padNumber))
	//	{
	//		chara->GenerateBall();
	//	}*/
	//}
	//else

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
	// �z���L���b�`����
	if (InputManager::Hold("Catch", padNumber))
	{
		if (chara->CanCatch())
			chara->Catch();
	}

	////////////////////////////////////////////////////
	// �W�����v����
	if (InputManager::Push("Jump", padNumber))
	{
		//if (chara->CanMove() && not chara->IsJumping())
			chara->Jump();
	}

	////////////////////////////////////////////////////
	// �X���C�f�B���O����
	if (InputManager::Hold("Slide", padNumber))
	{
		chara->Slide();
	}

	////////////////////////////////////////////////////
	// �e���|�[�g����
	if (InputManager::Push("Teleport", padNumber))
	{
		chara->TeleportToLastBall();
	}

	////////////////////////////////////////////////////
	// �L�����ړ����쏈��
	Camera* camera = CameraManager::GetCamera(chara->GetIndex());
	if (camera == nullptr)
		return;

	// �J�����̌����ɉ������X�e�B�b�N�̌X��
	Vector3 stick = InputManager::AnalogStick(padNumber) * MGetRotY(camera->transform->rotation.y);
	if (stick.GetLengthSquared() > 0.0f)
	{
		if (chara->CanMove())
			chara->Move(stick);
	}

	////////////////////////////////////////////////////
	// �^�b�N��
	if (chara->CanTackle())
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

	// �X�e�B�b�N���X���Ă��邩
	bool isTiltStick = (InputManager::AnalogStick(padNumber).GetLength() >= KeyDefine::STICK_DEADZONE);

	// �ړ��L�[�����͂���Ă��邩
	bool isPushKey = InputManager::Hold("Movement", padNumber);

	return (isTiltStick || isPushKey);
}
