#include "cameraState_Debug.h"
#include "camera.h"
#include "cameraDefine.h"

// ���ėp
#include "../Library/time.h"
#include "Util/Utils.h"

// ���ʂŕK�v�ȕ�
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

using namespace KeyDefine;
using namespace CameraDefine;

namespace {

	static const float MOVE_SPEED = 12.0f;	// ���ړ����x
	static const float SHIFT_SPEED = 6.0f;	// �c�ړ����x
	static const float ROT_SPEED = Math::DegToRad(3.0f);	// ��]���x
}

CameraState_Debug::CameraState_Debug(StateManager* p) : StateBase(p) {

	canMove = true;
}

CameraState_Debug::~CameraState_Debug() {

}

void CameraState_Debug::Update() {

	Camera* camera = Object<Camera>();
	if (camera == nullptr)
		return;

	// �E�N���b�N�����m
	if (InputManager::Push(KeyCode::RightClick)) {
		canMove = !canMove;

		// �ړ��\�ȏꍇ�A�}�E�X�J�[�\������ʒ����ɌŒ肷��
		if (canMove)
			MouseController::SetMouseMovement(MouseMovement::Fixed);
		// �ړ��s�\�ȏꍇ�A�}�E�X�J�[�\�������R�ɑ���ł���悤�ɂ���
		else
			MouseController::SetMouseMovement(MouseMovement::Free);
	}

	if (canMove)
		MoveProcess();
}

void CameraState_Debug::Draw() {

	Camera* camera = Object<Camera>();
	if (camera == nullptr)
		return;

	Vector3 targetPos = CAMERA_TARGET_DEF * camera->transform->Global().Matrix();
	camera->SetTarget(targetPos);
}

void CameraState_Debug::Resume() {

	canMove = true;
	MouseController::SetMouseMovement(MouseMovement::Fixed);
}

void CameraState_Debug::Suspend() {

	canMove = true;
}

void CameraState_Debug::MoveProcess() {

	Camera* camera = Object<Camera>();
	if (camera == nullptr)
		return;

	//====================================================================================================
	// ���}�E�X�ɂ��J�����̌����ύX

	camera->transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	camera->transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X���p�x�̐���
	camera->transform->rotation.x = min(max(camera->transform->rotation.x, CAMERA_ROT_X_MIN), CAMERA_ROT_X_MAX);

	//====================================================================================================
	// ���ړ�����

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * camera->transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	camera->transform->position += velocity;
}
