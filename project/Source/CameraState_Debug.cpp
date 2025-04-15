#include "cameraState_Debug.h"
#include "camera.h"
#include "cameraDefine.h"

// ◇汎用
#include "../Library/time.h"
#include "Util/Utils.h"

// ◇個別で必要な物
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"

using namespace KeyDefine;
using namespace CameraDefine;

namespace {

	static const float MOVE_SPEED = 12.0f;	// 横移動速度
	static const float SHIFT_SPEED = 6.0f;	// 縦移動速度
	static const float ROT_SPEED = Math::DegToRad(3.0f);	// 回転速度
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

	// 右クリックを検知
	if (InputManager::Push(KeyCode::RightClick)) {
		canMove = !canMove;

		// 移動可能な場合、マウスカーソルを画面中央に固定する
		if (canMove)
			MouseController::SetMouseMovement(MouseMovement::Fixed);
		// 移動不可能な場合、マウスカーソルを自由に操作できるようにする
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
	// ▼マウスによるカメラの向き変更

	camera->transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	camera->transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X軸角度の制限
	camera->transform->rotation.x = min(max(camera->transform->rotation.x, CAMERA_ROT_X_MIN), CAMERA_ROT_X_MAX);

	//====================================================================================================
	// ▼移動処理

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * camera->transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	camera->transform->position += velocity;
}
