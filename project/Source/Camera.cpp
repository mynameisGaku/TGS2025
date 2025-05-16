#include "camera.h"
#include "cameraManager.h"

// ���ėp
#include "../Library/myDxLib.h"
#include "../Library/time.h"
#include "../Source/Util/Utils.h"
#include "../Source/Stage.h"
#include <assert.h>

// ���X�e�[�g�֘A
#include "StateManager.h"

// ���R���|�[�l���g
#include "Component/Shake.h"

// ���ʂŕK�v�ȕ�
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"
#include "CameraDefineRef.h"

using namespace KeyDefine;
using namespace CameraDefine;

Camera::Camera() {

	Reset();
	
	AddComponent<Shake>();

	// fsm�̏�����
	fsm = new TinyFSM<Camera>(this);
	fsm->ChangeState(&Camera::DebugState); // �X�e�[�g��ύX
}

Camera::~Camera() {

	holder = nullptr;
	Function::DeletePointer(fsm);

	RemoveComponent<Shake>();
}

void Camera::Reset() {

	transform->position = Vector3(0.0f, 100.0f, -100.0f);
	transform->rotation = V3::ZERO;

	offset = CAMERADEFINE_REF.m_OffsetDef;
	offsetPrev = offset;

	holder = nullptr;
}

void Camera::Update() {

	if (fsm != nullptr)
		fsm->Update();

	Object3D::Update();
}

void Camera::Draw() {

	Object3D::Draw();

	const Transform globalTrs = transform->Global();

	Vector3 cameraPos = offset * globalTrs.RotationMatrix();

	// �U����K�p
	const Shake* shake = GetComponent<Shake>();
	if (shake != nullptr) {
		MATRIX mShakeTrs = shake->Matrix();
		cameraPos *= mShakeTrs;
	}

	// �e�ɒǏ]
	cameraPos += globalTrs.position;

	// ���L�҂ɒǏ]
	if (holder != nullptr) {
		cameraPos += holder->Global().position;
	}

	// �J������K�p
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraPos + Vector3(0, 0, 1) * globalTrs.RotationMatrix());
}

void Camera::ChangeState(void(Camera::* state)(FSMSignal)) {

    if (fsm == nullptr)
        return;

    fsm->ChangeState(state);
}

void Camera::ColCheckToTerrain() {

	static const Vector3 PUSH_OFFSET = V3::SetY(10.0f);
	const Vector3 cameraPosition = WorldPos();

	Vector3 hitPos = V3::ZERO;
	//bool hit = Stage::ColCheckGround(target, cameraPosition - PUSH_OFFSET, &hitPos);

	//if (hit) {
	//	Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * V3::UP;	// �n�ʂƂ̐ݒu�_
	//	Vector3 targePos = target * V3::HORIZONTAL;

	//	transform->position = terrainPos + targePos + PUSH_OFFSET;
	//}
}

Vector3 Camera::WorldPos() const {

	Vector3 globalPos = transform->Global().position;	// �J�����̐�΍��W
	Vector3 offsetRotAdap = OffsetRotAdaptor();			// ��]�s������������΍��W
	Vector3 holderPos = V3::ZERO;	// �ۗL�҂̍��W

	if (holder != nullptr)
		holderPos += holder->Global().position;

	Vector3 pos = globalPos + offsetRotAdap + holderPos;

	return pos;
}

Vector3 Camera::TargetLay() const {

	return Vector3(0, 0, 1) * transform->Global().RotationMatrix();
}

void Camera::moveProcess()
{
	//====================================================================================================
	// ���}�E�X�ɂ��J�����̌����ύX

	transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X���p�x�̐���
	transform->rotation.x = min(max(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min), CAMERADEFINE_REF.m_RotX_Max);

	//====================================================================================================
	// ���ړ�����

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	transform->position += velocity;
}

void Camera::operationByMouse(int type) {

	Vector2 addRot = V2::ZERO;	// ���Z�����]��

	MouseController::MouseInfo mouse = MouseController::Info();	// �}�E�X�̏��

	switch (type) {
	case 0:	addRot.x = (mouse.moveY * mouse.sensitivity.y) * Math::DegToRad(0.1f);	break;
	case 1:	addRot.y = (mouse.moveX * mouse.sensitivity.x) * Math::DegToRad(0.1f);	break;

	default:
		addRot.x = (mouse.moveY * mouse.sensitivity.y) * Math::DegToRad(0.1f);
		addRot.y = (mouse.moveX * mouse.sensitivity.x) * Math::DegToRad(0.1f);
		break;
	}

	// ���������߂��Ȃ��l�ɁA������������
	transform->rotation.x += min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	transform->rotation.y += min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
}

void Camera::operationByStick(int type) {

	Vector2 addRot = V2::ZERO;	// ���Z�����]��
	Vector2 rightStick = PadController::NormalizedRightStick();

	switch (type) {
	case 0:	addRot.x = (rightStick.y * PadController::StickSensitivity().y) * Math::DegToRad(-1.0f); break;
	case 1:	addRot.y = (rightStick.x * PadController::StickSensitivity().x) * Math::DegToRad(1.0f);	break;
	default:
		addRot.x = (rightStick.y * PadController::StickSensitivity().y) * Math::DegToRad(-1.0f);
		addRot.y = (rightStick.x * PadController::StickSensitivity().x) * Math::DegToRad(1.0f);
		break;
	}
	// ���������߂��Ȃ��l�ɁA������������
	addRot.x = min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	addRot.y = min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);

	transform->rotation.x += addRot.x;
	transform->rotation.y += addRot.y;
}
