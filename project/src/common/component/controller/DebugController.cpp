#include "DebugController.h"

#include "src/util/time/GameTime.h"

#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/chara/CharaBase.h"
#include "src/util/fsm/StateManager.h"

#include "src/common/component/physics/Physics.h"

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
	// �{�[�������鏈��
	if (PadController::NormalizedRightTrigger() > KeyDefine::TRIGGER_DEADZONE)
	{
		if (not isPush)
		{
			chara->GenerateBall();
			chara->ThrowHomingBall();
			isPush = true;
		}
	}
	else isPush = false;

	////////////////////////////////////////////////////
	// �z���L���b�`����
	if (PadController::NormalizedLeftTrigger() > KeyDefine::TRIGGER_DEADZONE)
	{
		chara->Catch();
	}

	////////////////////////////////////////////////////
	// �W�����v����
	if (IsPressButton(KeyDefine::KeyCode::ButtonA, KeyDefine::Begin))
	{
		chara->Jump();
	}

	////////////////////////////////////////////////////
	// �X���C�f�B���O����
	if (IsPressButton(KeyDefine::KeyCode::RightThumb, KeyDefine::Stationary))
	{
		chara->Slide();
	}

	////////////////////////////////////////////////////
	// �L�����ړ����쏈��
	if (chara == nullptr || IsMoveButton() == false)
		return;

	Camera* camera = CameraManager::GetCamera(chara->GetIndex());
	if (camera == nullptr)
		return;

	// �J�����̌����ɉ������X�e�B�b�N�̌X��
    Vector2 leftStick = PadController::NormalizedLeftStick(padNumber);
	Vector3 virtualStick = Vector3(leftStick.x, 0.0f, leftStick.y);
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

	// �X�e�B�b�N���X���Ă��邩
	bool isTiltStick = (PadController::NormalizedLeftStick(padNumber).GetLength() >= KeyDefine::STICK_DEADZONE);

	return (isTiltStick);
}
