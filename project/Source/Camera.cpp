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
using namespace CameraDefine;

Camera::Camera() {

	Reset();

	cameraWork = nullptr;
	
	AddComponent<Shake>();

	// fsm�̏�����
	fsm = new TinyFSM<Camera>(this);
    fsm->RegisterStateName(&Camera::DebugState, "DebugState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
    fsm->RegisterStateName(&Camera::ChaseState, "ChaseState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	fsm->ChangeState(&Camera::DebugState); // �X�e�[�g��ύX
	
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
	transform->rotation = V3::ZERO;

	offset = CAMERA_OFFSET_DEF;
	target = CAMERA_TARGET_DEF;
	holder = nullptr;
}

void Camera::Update() {

	if (fsm != nullptr)
		fsm->Update();

	Object3D::Update();
}

void Camera::Draw() {

	Object3D::Draw();

	if (fsm != nullptr)
		fsm->ImGuiDebugRender();

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
}

void Camera::ChangeState(void(Camera::* state)(FSMSignal)) {

    if (fsm == nullptr)
        return;

    fsm->ChangeState(state);
}

void Camera::ColCheckToTerrain() {

	Vector3 hitPos = V3::ZERO;
	Vector3 cameraPosition = WorldPos();

	if (Stage::ColCheckGround(target, cameraPosition - V3::SetY(10.0f), &hitPos)) {
		Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * V3::UP;	// �n�ʂƂ̐ݒu�_
		Vector3 targePos = target * V3::HORIZONTAL;

		transform->position = terrainPos + targePos + V3::SetY(10.0f);
	}
}

void Camera::MoveProcess()
{
	//====================================================================================================
	// ���}�E�X�ɂ��J�����̌����ύX

	transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X���p�x�̐���
	transform->rotation.x = min(max(transform->rotation.x, CAMERA_ROT_X_MIN), CAMERA_ROT_X_MAX);
	
    target = transform->position + CAMERA_TARGET_DEF * transform->RotationMatrix();

	//====================================================================================================
	// ���ړ�����

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	transform->position += velocity;
}

void Camera::OperationByMouse() {

	Vector2 addRot = V2::ZERO;	// ���Z�����]��

	MouseInfo mouse = MouseController::Info();	// �}�E�X�̏��

	addRot.x = (mouse.moveY * mouse.sensitivity.y) * Math::DegToRad(0.1f);
	addRot.y = (mouse.moveX * mouse.sensitivity.x) * Math::DegToRad(0.1f);

	// ���������߂��Ȃ��l�ɁA������������
	transform->rotation.x += min(max(addRot.x, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);
	transform->rotation.y += min(max(addRot.y, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);
}

void Camera::OperationByStick() {

	Vector2 addRot = V2::ZERO;	// ���Z�����]��
	Vector2 rightStick = PadController::NormalizedRightStick();

	addRot.x = (rightStick.y * PadController::StickSensitivity().y) * Math::DegToRad(-1.0f);
	addRot.y = (rightStick.x * PadController::StickSensitivity().x) * Math::DegToRad(1.0f);

	// ���������߂��Ȃ��l�ɁA������������
	addRot.x = min(max(addRot.x, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);
	addRot.y = min(max(addRot.y, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);

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
	Vector3 holderPos = V3::ZERO;	// �ۗL�҂̍��W

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
