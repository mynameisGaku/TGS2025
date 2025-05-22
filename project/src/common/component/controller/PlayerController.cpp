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
	// �{�[�������鏈��

	// ToDo : �`���[�W���Ή��Ȃ̂őΉ������邒
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
	// �z���L���b�`����
	if (IsPressButton(KeyDefine::KeyCode::X, KeyDefine::Stationary))
	{
		chara->Catch();
	}

	////////////////////////////////////////////////////
	// �W�����v����
	if (IsPressButton(KeyDefine::KeyCode::Space, KeyDefine::Begin))
	{
		chara->Jump();
	}

	////////////////////////////////////////////////////
	// �X���C�f�B���O����
	if (IsPressButton(KeyDefine::KeyCode::LeftShift, KeyDefine::Stationary) ||
		IsPressButton(KeyDefine::KeyCode::LeftControl, KeyDefine::Stationary) ||
		IsPressButton(KeyDefine::KeyCode::C, KeyDefine::Stationary))
	{
		chara->Slide();
	}

	////////////////////////////////////////////////////
	// �e���|�[�g����
	if (IsPressButton(KeyDefine::KeyCode::E, KeyDefine::Begin))
	{
		chara->TeleportToLastBall();
	}

	////////////////////////////////////////////////////
	// �L�����ړ����쏈��
	if (chara == nullptr || IsMoveButton() == false)
		return;

	Camera* camera = CameraManager::MainCamera();
	if (camera == nullptr)
		return;

	// �J�����̌����ɉ������X�e�B�b�N�̌X��
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

	// �X�e�B�b�N���X���Ă��邩
	bool isTiltStick = (InputManager::AnalogStick(padNumber).GetLength() >= KeyDefine::STICK_DEADZONE);

	// �ړ��L�[�����͂���Ă��邩
	bool isPushKey = InputManager::Hold("Movement", padNumber);

	return (isTiltStick || isPushKey);
}
