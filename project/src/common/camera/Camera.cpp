#include "src/common/camera/Camera.h"
#include "src/common/camera/CameraManager.h"

// ◇汎用
#include "framework/myDxLib.h"
#include "src/util/time/GameTime.h"
#include "src/Util/ptr/PtrUtil.h"
#include "src/common/stage/Stage.h"
#include "src/util/easing/EasingUtils.h"
#include <assert.h>

// ◇ステート関連
#include "src/util/fsm/StateManager.h"

// ◇個別で必要な物
#include "src/util/input/InputManager.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/setting/window/WindowSetting.h"

using namespace KeyDefine;
using namespace CameraDefine;

Camera::Camera() {

	Reset();

	cameraWork = nullptr;

	m_pShake = AddComponent<Shake>();
	m_pShake->Init(this);

	// fsmの初期化
	fsm = new TinyFSM<Camera>(this);
	fsm->RegisterStateName(&Camera::DebugState, "DebugState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	//fsm->RegisterStateName(&Camera::ChaseState, "ChaseState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	fsm->ChangeState(&Camera::DebugState); // ステートを変更

	cameraCone.transform = *transform;
	cameraCone.range = CAMERADEFINE_REF.m_ConeRange;
	cameraCone.angle = CAMERADEFINE_REF.m_ConeAngle;

	m_FollowerChara = nullptr;
	m_TargetChara = nullptr;
	
	m_IsView = true;
	m_DrawFlag = false;
	
	m_AnimData = CameraAnimData(); // カメラアニメーションデータの初期化

	m_CameraRotMat = MGetIdent();

	//cameraWork = new CsvReader("data/csv/CameraWork.csv");
}

Camera::~Camera() {

	holder = nullptr;
	PtrUtil::SafeDelete(fsm);
	PtrUtil::SafeDelete(cameraWork);

	RemoveComponent<Shake>();
}

void Camera::Reset() {

	transform->position = Vector3(0.0f, 100.0f, -100.0f);
	transform->rotation = Vector3::Zero;

	m_PositionPrev = transform->position;
	m_RotationPrev = transform->position;

	m_Offset = CAMERADEFINE_REF.m_OffsetDef;
	m_OffsetPrev = m_Offset;
	
	m_Target = CAMERADEFINE_REF.m_TargetDef;
	m_TargetPrev = m_Offset;

	m_EasingTime = 0.0f;
	m_TargetTransitionTime = 0.0f;

	holder = nullptr;
}

void Camera::Update() {

	cameraCone.transform = *transform;

	if (fsm != nullptr)
		fsm->Update();

	UpdateAnimation();

	Object3D::Update();

	m_DrawFlag = false;
}

void Camera::Draw() {

	if (m_DrawFlag)
		return;

	if (not m_IsView) {
		DrawVirtualCamera();
		return;
	}

	Object3D::Draw();

	MATRIX mShake = m_pShake->Matrix();	// 振動用行列

	Transform globalTrs = transform->Global();

	Vector3 cameraPos = WorldPos() * mShake;
	Vector3 m_TargetPos = m_Target * mShake;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		m_TargetPos += holder->Global().position;
	}

	SetCameraPositionAndTargetAndUpVec(cameraPos, m_TargetPos, Vector3::TransformCoord(Vector3::UnitY,m_CameraRotMat));
	//SetCameraPositionAndTarget_UpVecY(cameraPos, m_TargetPos);
	
	m_DrawFlag = true;
}

void Camera::DrawVirtualCamera() {

	Transform globalTrs = transform->Global();

	Vector3 cameraPos = WorldPos();
	Vector3 m_TargetPos = m_Target;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		m_TargetPos += holder->Global().position;
	}

	DrawCapsule3D(cameraPos, m_TargetPos, 8.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
}

void Camera::ChangeState(void(Camera::* state)(FSMSignal)) {

    if (fsm == nullptr)
        return;

    fsm->ChangeState(state);
}

void Camera::ColCheckToTerrain() {

	Vector3 hitPos = Vector3::Zero;	// 当たった座標
	Vector3 cameraPos = WorldPos();	// カメラの座標
	Vector3 lay = Vector3::SetY(-10.0f);

	if (Stage::ColCheckGround(m_Target, cameraPos + lay, &hitPos)) {
		Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * Vector3::UnitY;	// 地面との設置点
		Vector3 targePos = m_Target * Vector3::UnitY;

		transform->position = terrainPos + targePos - lay;
	}
}

void Camera::MoveProcess()
{
	//====================================================================================================
	// ▼マウスによるカメラの向き変更

	transform->rotation.x += (MouseController::Info().Move().y * MathUtil::ToRadians(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * MathUtil::ToRadians(1.0f));

	// X軸角度の制限
	transform->rotation.x = min(max(transform->rotation.x, CAMERADEFINE_REF.m_RotX_Min), CAMERADEFINE_REF.m_RotX_Max);
	
    m_Target = transform->position + CAMERADEFINE_REF.m_TargetDef * transform->RotationMatrix();

	//====================================================================================================
	// ▼移動処理

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	transform->position += velocity;
}

void Camera::OperationByMouse(int type) {

	Vector2 addRot = Vector2::Zero;	// 加算する回転量

	MouseController::MouseInfo mouse = MouseController::Info();	// マウスの情報

	switch (type) {
	case 0:	addRot.x = (mouse.moveY * mouse.sensitivity.y) * MathUtil::ToRadians(0.1f);	break;
	case 1:	addRot.y = (mouse.moveX * mouse.sensitivity.x) * MathUtil::ToRadians(0.1f);	break;

	default:
		addRot.x = (mouse.moveY * mouse.sensitivity.y) * MathUtil::ToRadians(0.1f);
		addRot.y = (mouse.moveX * mouse.sensitivity.x) * MathUtil::ToRadians(0.1f);
		break;
	}

	// 勢いがつき過ぎない様に、制限をかける
	transform->rotation.x += min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	transform->rotation.y += min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
}

void Camera::OperationByStick(int padNumber, int type) {

	Vector2 addRot = Vector2::Zero;	// 加算する回転量
	Vector2 rightStick = PadController::NormalizedRightStick(padNumber);

	switch (type) {
	case 0:	addRot.x = (rightStick.y * PadController::StickSensitivity().y) * MathUtil::ToRadians(-1.0f);break;
	case 1:	addRot.y = (rightStick.x * PadController::StickSensitivity().x) * MathUtil::ToRadians(1.0f);	break;
	default:
		addRot.x = (rightStick.y * PadController::StickSensitivity().y) * MathUtil::ToRadians(-1.0f);
		addRot.y = (rightStick.x * PadController::StickSensitivity().x) * MathUtil::ToRadians(1.0f);
		break;
	}
	// 勢いがつき過ぎない様に、制限をかける
	addRot.x = min(max(addRot.x, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);
	addRot.y = min(max(addRot.y, -CAMERADEFINE_REF.m_RotSpeedLimit), CAMERADEFINE_REF.m_RotSpeedLimit);

	transform->rotation.x += addRot.x;
	transform->rotation.y += addRot.y;
}

void Camera::UpdateAnimation() {

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

Vector3 Camera::WorldPos() const {

	Vector3 globalPos = transform->Global().position;	// カメラの絶対座標
	Vector3 m_OffsetRotAdap = OffsetRotAdaptor();			// 回転行列をかけた相対座標
	Vector3 holderPos = Vector3::Zero;	// 保有者の座標

	if (holder != nullptr)
		holderPos += holder->Global().position;

	Vector3 pos = globalPos + m_OffsetRotAdap + holderPos;

	return pos;
}

Vector3 Camera::TargetLay() const {

	Vector3 cameraPos = m_Offset * transform->Matrix();
	Vector3 m_TargetPos = m_Target;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		m_TargetPos += holder->Global().position;
	}

	return m_TargetPos - cameraPos;
}

bool Camera::IsFrontView(const Vector3& pos) const {

	Vector3 cameraPos = WorldPos();	// カメラの座標
	Vector3 toPos = pos - cameraPos;// カメラからのベクトル
	Vector3 cameraDir = transform->Global().Forward();	// カメラの向きベクトル

	float dot = Vector3::Dot(toPos, cameraDir);	// 内積を計算
	if (dot > 0.0f)	// 内積が正なら、カメラの前方にある
		return true;

	return false;
}

bool Camera::IsRightView(const Vector3& pos) const {

	Vector3 cameraPos = WorldPos();	// カメラの座標
	Vector3 toPos = pos - cameraPos;// カメラからのベクトル
	Vector3 cameraDir = transform->Global().Right();	// カメラの向きベクトル

	float dot = Vector3::Dot(toPos, cameraDir);	// 内積を計算
	if (dot > 0.0f)	// 内積が正なら、カメラの前方にある
		return true;

	return false;

	return false;
}
