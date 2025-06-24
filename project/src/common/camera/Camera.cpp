#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraManager.h"

// ���ėp
#include "framework/myDxLib.h"
#include "src/util/time/GameTime.h"
#include "src/Util/ptr/PtrUtil.h"
#include "src/common/stage/Stage.h"
#include "src/common/stage/StageObjectManager.h"
#include "src/util/easing/EasingUtils.h"
#include <assert.h>

// ���X�e�[�g�֘A
#include "src/util/fsm/StateManager.h"

// ���ʂŕK�v�ȕ�
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/setting/window/WindowSetting.h"

using namespace KeyDefine;
using namespace CameraDefine;

Camera::Camera() {

	Reset();

	m_pShake = AddComponent<Shake>();
	m_pShake->Init(this);

	// m_Fsm�̏�����
	m_Fsm = new TinyFSM<Camera>(this);
	m_Fsm->RegisterStateName(&Camera::DebugState, "DebugState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	//m_Fsm->RegisterStateName(&Camera::ChaseState, "ChaseState"); // ���̍s���̓f�o�b�O�p�B�֐��|�C���^�̓R���p�C����Ɋ֐������ێ�����Ȃ��̂ŁA�v���t�@�C�����O����ɂ͂��̍s�����K�{�B
	m_Fsm->ChangeState(&Camera::DebugState); // �X�e�[�g��ύX
	
	m_AnimData = CameraAnimData(); // �J�����A�j���[�V�����f�[�^�̏�����

	m_CameraWork = nullptr;
	//m_CameraWork = new CsvReader("data/csv/CameraWork.csv");
}

Camera::~Camera() {

	PtrUtil::SafeDelete(m_Fsm);
	PtrUtil::SafeDelete(m_CameraWork);

	RemoveComponent<Shake>();
}

void Camera::Reset() {

	transform->position = Vector3(0.0f, 100.0f, -100.0f);
	transform->rotation = Vector3::Zero;

	m_PositionPrev = transform->position;
	m_RotationPrev = transform->rotation;

	m_Offset = CAMERADEFINE_REF.m_OffsetDef;
	m_OffsetPrev = m_Offset;
	
	m_Target = CAMERADEFINE_REF.m_TargetDef;
	m_TargetPrev = m_Offset;

	m_EasingTime = 0.0f;
	m_TargetTransitionTime = 0.0f;

	m_CameraRotMat = MGetIdent();

	m_CameraCone.transform = *transform;
	m_CameraCone.range = CAMERADEFINE_REF.m_ConeRange;
	m_CameraCone.angle = CAMERADEFINE_REF.m_ConeAngle;

	m_pHolder = nullptr;
	m_pFollowerChara = nullptr;
	m_pTargetChara = nullptr;

	m_IsView = true;
	m_DrawFlag = false;
}

void Camera::Update() {

	m_CameraCone.transform = *transform;

	if (m_Fsm != nullptr)
		m_Fsm->Update();

	updateAnimation();

	Object3D::Update();

	m_DrawFlag = false;
}

void Camera::Draw() {

	// ���ɕ`��ςȂ�
	if (m_DrawFlag)
		return;

	// �`��̗L��
	if (not m_IsView) {
		drawVirtualCamera();
		return;
	}

	Object3D::Draw();

	// �J�����`��
	rendering();
	
	m_DrawFlag = true;	// �`�抮��
}

void Camera::drawVirtualCamera() {

	Vector3 cameraPos = WorldPos();
	Vector3 targetPos = m_Target;

	if (m_pHolder != nullptr)
	{
		cameraPos += m_pHolder->Global().position;
		targetPos += m_pHolder->Global().position;
	}

	DrawCapsule3D(cameraPos, targetPos, 8.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
}

void Camera::ChangeState(void(Camera::* state)(FSMSignal)) {

    if (m_Fsm == nullptr)
        return;

    m_Fsm->ChangeState(state);
}

void Camera::rendering() {

	Vector3 cameraPos = WorldPos() * m_pShake->Matrix();
	Vector3 targetPos = m_Target * m_pShake->Matrix();

	// �ۗL�҂����݂���ꍇ
	if (m_pHolder != nullptr)
	{
		cameraPos += m_pHolder->Global().position;
		targetPos += m_pHolder->Global().position;
	}

	Vector3 hitPos = Vector3::Zero;
	Vector3 apprach = (targetPos - cameraPos).Normalize() * 16.0f;

	if (StageObjectManager::CollCheckLine(cameraPos, targetPos, &hitPos))
		cameraPos = hitPos + apprach;

	//cameraPos = StageObjectManager::CollCheckCapsule_Hitpos(cameraPos, targetPos, 32.0f, &hitPos);

	SetCameraPositionAndTargetAndUpVec(cameraPos, targetPos, Vector3::TransformCoord(Vector3::UnitY, m_CameraRotMat));
}

Vector3 Camera::colCheckToTerrain(const Vector3& begin, const Vector3& end, Vector3* hitPos) {

	Vector3 cameraPos = WorldPos();
	Vector3 apprach = (end - begin).Normalize() * 100.0f;

	if (Stage::ColCheckGround(begin, end, hitPos)) {
		cameraPos = apprach;
		if (hitPos != nullptr)
			cameraPos += *hitPos;
	}

	return cameraPos;
}

Vector3 Camera::collCheckCapsule_Hitpos(const Vector3& begin, const Vector3& end, Vector3* hitPos) {

	Vector3 cameraPos = WorldPos();
	Vector3 apprach = (end - begin).Normalize() * 100.0f;

	if (StageObjectManager::CollCheckCapsule_Hitpos(begin, end, 32.0f, hitPos)) {
		cameraPos = apprach;
		if (hitPos != nullptr)
			cameraPos += *hitPos;
	}

	return cameraPos;
}

void Camera::moveProcess()
{
	//====================================================================================================
	// ���}�E�X�ɂ��J�����̌����ύX

	transform->rotation.x += (MouseController::Info().Move().y * MathUtil::ToRadians(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * MathUtil::ToRadians(1.0f));

	// X���p�x�̐���
	transform->rotation.x = min(max(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min), CAMERADEFINE_REF.m_RotX_Max);
	
    m_Target = transform->position + CAMERADEFINE_REF.m_TargetDef * transform->RotationMatrix();

	//====================================================================================================
	// ���ړ�����

	Vector3 velocity = (InputManager::AnalogStick() * CAMERADEFINE_REF.m_MoveSpeed) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += CAMERADEFINE_REF.m_ShiftSpeed;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= CAMERADEFINE_REF.m_ShiftSpeed;

	transform->position += velocity;
}

void Camera::operationByMouse(ViewPointShift type) {

	Vector2 addRot = Vector2::Zero;	// ���Z�����]��

	MouseController::MouseInfo mouse = MouseController::Info();	// �}�E�X�̏��

	switch (type) {
	case ViewPointShift::Horizontal:addRot.x = (mouse.moveY * mouse.sensitivity.y) * MathUtil::ToRadians(0.1f);	break;
	case ViewPointShift::Vertical:	addRot.y = (mouse.moveX * mouse.sensitivity.x) * MathUtil::ToRadians(0.1f);	break;
	case ViewPointShift::All:
		addRot.x = (mouse.moveY * mouse.sensitivity.y) * MathUtil::ToRadians(0.1f);
		addRot.y = (mouse.moveX * mouse.sensitivity.x) * MathUtil::ToRadians(0.1f);
		break;
	}

	// ���������߂��Ȃ��l�ɁA������������
	transform->rotation.x += min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	transform->rotation.y += min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
}

void Camera::operationByStick(int padNumber, ViewPointShift type) {

	Vector2 addRot = Vector2::Zero;	// ���Z�����]��
	Vector2 rightStick = PadController::NormalizedRightStick(padNumber);

	switch (type) {
	case ViewPointShift::Horizontal:addRot.x = (rightStick.y * PadController::StickSensitivity().y) * MathUtil::ToRadians(-1.0f);break;
	case ViewPointShift::Vertical:	addRot.y = (rightStick.x * PadController::StickSensitivity().x) * MathUtil::ToRadians(1.0f);	break;
	case ViewPointShift::All:
		addRot.x = (rightStick.y * PadController::StickSensitivity().y) * MathUtil::ToRadians(-1.0f);
		addRot.y = (rightStick.x * PadController::StickSensitivity().x) * MathUtil::ToRadians(1.0f);
		break;
	}
	// ���������߂��Ȃ��l�ɁA������������
	addRot.x = min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	addRot.y = min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);

	transform->rotation.x += addRot.x;
	transform->rotation.y += addRot.y;
}

void Camera::updateAnimation() {

	if (not m_AnimData.isPlaying)
		return;

	m_AnimData.Update();

	switch (m_AnimData.state) {
	case CameraAnimState::Begin_To_End:
		if (m_AnimData.useAnim)
			m_Offset = EasingFunc::Linear(m_AnimData.beginToEndSec, m_AnimData.beginToEndSec_Max, m_AnimData.begin, m_AnimData.end);

		if (m_AnimData.useTarget)
			m_Target = EasingFunc::Linear(m_AnimData.beginToEndSec, m_AnimData.beginToEndSec_Max, m_AnimData.beginTarget, m_AnimData.endTarget);

		if (m_AnimData.useMatrix) {
			Vector3 rot = EasingFunc::Linear(m_AnimData.beginToEndSec, m_AnimData.beginToEndSec_Max, m_AnimData.beginRotMatrix, m_AnimData.endRotMatrix);
			m_CameraRotMat = rot.ToRotationMatrix();
		}

		break;

	case CameraAnimState::End_To_Begin:
		if (m_AnimData.useAnim)
			m_Offset = EasingFunc::Linear(m_AnimData.endToBeginSec, m_AnimData.endToBeginSec_Max, m_AnimData.end, m_AnimData.begin);

		if (m_AnimData.useTarget)
			m_Target = EasingFunc::Linear(m_AnimData.endToBeginSec, m_AnimData.endToBeginSec_Max, m_AnimData.endTarget, m_AnimData.beginTarget);

		if (m_AnimData.useMatrix) {
			Vector3 rot = EasingFunc::Linear(m_AnimData.endToBeginSec, m_AnimData.endToBeginSec_Max, m_AnimData.endRotMatrix, m_AnimData.beginRotMatrix);
			m_CameraRotMat = rot.ToRotationMatrix();
		}
		break;

	default:
		break;
	}
}

void Camera::SetPerformance(const std::string& perfType) {

	//stateManager->ChangeState(State::sPerformance);
	//dynamic_cast<CameraState_Performance*>(stateManager->State(State::sPerformance))->SetCameraWork(perfType);
}

void Camera::SetAnimation(const CameraAnimData& animData) {

	m_AnimData = animData;
}

const Vector3 Camera::WorldPos() const {

	Vector3 globalPos = transform->Global().position;	// �J�����̐�΍��W
	Vector3 m_OffsetRotAdap = OffsetRotAdaptor();			// ��]�s������������΍��W
	Vector3 m_pHolderPos = Vector3::Zero;	// �ۗL�҂̍��W

	if (m_pHolder != nullptr)
		m_pHolderPos += m_pHolder->Global().position;

	Vector3 pos = globalPos + m_OffsetRotAdap + m_pHolderPos;

	return pos;
}

const Vector3 Camera::TargetLay() const {

	Vector3 cameraPos = m_Offset * transform->Matrix();
	Vector3 m_TargetPos = m_Target;

	if (m_pHolder != nullptr) {
		cameraPos += m_pHolder->Global().position;
		m_TargetPos += m_pHolder->Global().position;
	}

	return m_TargetPos - cameraPos;
}

bool Camera::IsFrontView(const Vector3& pos) const {

	Vector3 cameraPos = WorldPos();	// �J�����̍��W
	Vector3 toPos = pos - cameraPos;// �J��������̃x�N�g��
	Vector3 cameraDir = transform->Global().Forward();	// �J�����̌����x�N�g��

	float dot = Vector3::Dot(toPos, cameraDir);	// ���ς��v�Z
	if (dot > 0.0f)	// ���ς����Ȃ�A�J�����̑O���ɂ���
		return true;

	return false;
}

bool Camera::IsRightView(const Vector3& pos) const {

	Vector3 cameraPos = WorldPos();	// �J�����̍��W
	Vector3 toPos = pos - cameraPos;// �J��������̃x�N�g��
	Vector3 cameraDir = transform->Global().Right();	// �J�����̌����x�N�g��

	float dot = Vector3::Dot(toPos, cameraDir);	// ���ς��v�Z
	if (dot > 0.0f)	// ���ς����Ȃ�A�J�����̑O���ɂ���
		return true;

	return false;

	return false;
}
