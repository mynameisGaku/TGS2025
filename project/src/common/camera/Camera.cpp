#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraManager.h"

// ���ėp
#include "framework/myDxLib.h"
#include "src/util/time/GameTime.h"
#include "src/Util/Utils.h"
#include "src/common/stage/Stage.h"
#include <assert.h>

// ���X�e�[�g�֘A
#include "src/util/fsm/StateManager.h"

// ���R���|�[�l���g
#include "src/common/component/shake/Shake.h"

// ���ʂŕK�v�ȕ�
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include "src/reference/camera/CameraDefineRef.h"

using namespace KeyDefine;
using namespace CameraDefine;

Camera::Camera() {

	Reset();

	cameraWork = nullptr;

	AddComponent<Shake>();

	// fsm�̏�����
	fsm = new TinyFSM<Camera>(this);
	fsm->RegisterStateName(&Camera::DebugState, "DebugState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	//fsm->RegisterStateName(&Camera::ChaseState, "ChaseState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	fsm->ChangeState(&Camera::DebugState); // �X�e�[�g��ύX

	cameraCone.transform = *transform;
	cameraCone.range = CAMERADEFINE_REF.m_ConeRange;
	cameraCone.angle = CAMERADEFINE_REF.m_ConeAngle;

	//cameraWork = new CsvReader("data/csv/CameraWork.csv");
}

Camera::~Camera() {

	holder = nullptr;
	Function::DeletePointer(fsm);
	Function::DeletePointer(cameraWork);

	RemoveComponent<Shake>();
}

void Camera::Reset() {

	transform->position = Vector3(0.0f, 100.0f, -100.0f);
	transform->rotation = Vector3::Zero;

	offset = CAMERADEFINE_REF.m_OffsetDef;
	target = CAMERADEFINE_REF.m_TargetDef;
	offsetPrev = offset;
	targetPrev = target;

	holder = nullptr;
}

void Camera::Update() {

	cameraCone.transform = *transform;

	if (fsm != nullptr)
		fsm->Update();

	Object3D::Update();
}

void Camera::Draw() {

	Object3D::Draw();

	MATRIX mShakeTrs = MGetIdent();	// �U���p�s��

	Shake* shake = GetComponent<Shake>();
	if (shake != nullptr)
		mShakeTrs = shake->Matrix();

	Transform globalTrs = transform->Global();

	Vector3 cameraPos = globalTrs.position + offset * globalTrs.RotationMatrix() * mShakeTrs;
	Vector3 targetPos = target * mShakeTrs;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		targetPos += holder->Global().position;
	}

	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

	//Vector3 coneLineL = Vector3::SetZ(cameraCone.range * 0.1f) * MGetRotY(Math::DegToRad(cameraCone.angle * 0.5f)) * globalTrs.RotationMatrix();
	//Vector3 coneLineR = Vector3::SetZ(cameraCone.range * 0.1f) * MGetRotY(Math::DegToRad(cameraCone.angle * -0.5f)) * globalTrs.RotationMatrix();
	//DrawLine3D(cameraPos, cameraPos + coneLineL, 0xFF0000);
	//DrawLine3D(cameraPos, cameraPos + coneLineR, 0xFF0000);
	//DrawSphere3D(targetPos, 8.0f, 16, 0x00FF00, 0xFFFFFF, false);
	//DrawCapsule3D(cameraPos, cameraPos + coneLineL, 8.0f, 16, 0xFFFF00, 0xFFFFFF, false);
	//DrawCapsule3D(cameraPos, cameraPos + coneLineR, 8.0f, 16, 0x00FFFF, 0xFFFFFF, false);
}

void Camera::ChangeState(void(Camera::* state)(FSMSignal)) {

    if (fsm == nullptr)
        return;

    fsm->ChangeState(state);
}

void Camera::ColCheckToTerrain() {

	Vector3 hitPos = Vector3::Zero;
	Vector3 cameraPosition = WorldPos();

	if (Stage::ColCheckGround(target, cameraPosition - Vector3::SetY(10.0f), &hitPos)) {
		Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * Vector3::UnitY;	// �n�ʂƂ̐ݒu�_
		Vector3 targePos = target * Vector3::UnitY;

		transform->position = terrainPos + targePos + Vector3::SetY(10.0f);
	}
}

void Camera::MoveProcess()
{
	//====================================================================================================
	// ���}�E�X�ɂ��J�����̌����ύX

	transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X���p�x�̐���
	transform->rotation.x = min(max(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min), CAMERADEFINE_REF.m_RotX_Max);
	
    target = transform->position + CAMERADEFINE_REF.m_TargetDef * transform->RotationMatrix();

	//====================================================================================================
	// ���ړ�����

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	transform->position += velocity;
}

void Camera::OperationByMouse(int type) {

	Vector2 addRot = Vector2::Zero;	// ���Z�����]��

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

void Camera::OperationByStick(int type) {

	Vector2 addRot = Vector2::Zero;	// ���Z�����]��
	Vector2 rightStick = PadController::NormalizedRightStick();

	switch (type) {
	case 0:	addRot.x = (rightStick.y * PadController::StickSensitivity().y) * Math::DegToRad(-1.0f);break;
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

void Camera::SetPerformance(const std::string& perfType) {

	//stateManager->ChangeState(State::sPerformance);
	//dynamic_cast<CameraState_Performance*>(stateManager->State(State::sPerformance))->SetCameraWork(perfType);
}

Vector3 Camera::WorldPos() const {

	Vector3 globalPos = transform->Global().position;	// �J�����̐�΍��W
	Vector3 offsetRotAdap = OffsetRotAdaptor();			// ��]�s������������΍��W
	Vector3 holderPos = Vector3::Zero;	// �ۗL�҂̍��W

	if (holder != nullptr)
		holderPos += holder->Global().position;

	Vector3 pos = globalPos + offsetRotAdap + holderPos;

	return pos;
}

Vector3 Camera::TargetLay() const {

	Vector3 cameraPos = offset * transform->Matrix();
	Vector3 targetPos = target;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		targetPos += holder->Global().position;
	}

	return targetPos - cameraPos;
}
