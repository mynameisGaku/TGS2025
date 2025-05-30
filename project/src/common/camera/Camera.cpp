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

	m_TargetChara = nullptr;
	isView = true;
	drawFlag = false;

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

	offset = CAMERADEFINE_REF.m_OffsetDef;
	target = CAMERADEFINE_REF.m_TargetDef;
	offsetAfter = offset;
	targetAfter = target;

	m_TargetTransitionTime = 0.0f;

	holder = nullptr;
}

void Camera::Update() {

	cameraCone.transform = *transform;

	if (fsm != nullptr)
		fsm->Update();

	UpdateOffsetLeap();
	UpdateTargetLeap();
	UpdateAnimation();

	Object3D::Update();

	drawFlag = false;
}

void Camera::Draw() {

	if (drawFlag)
		return;

	if (not isView) {
		DrawVirtualCamera();
		return;
	}

	Object3D::Draw();

	MATRIX mShake = m_pShake->Matrix();	// 振動用行列

	Transform globalTrs = transform->Global();

	Vector3 cameraPos = WorldPos() * mShake;
	Vector3 targetPos = target * mShake;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		targetPos += holder->Global().position;
	}

	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

	drawFlag = true;
}

void Camera::DrawVirtualCamera() {

	Transform globalTrs = transform->Global();

	Vector3 cameraPos = WorldPos();
	Vector3 targetPos = target;

	if (holder != nullptr) {
		cameraPos += holder->Global().position;
		targetPos += holder->Global().position;
	}

	DrawCapsule3D(cameraPos, targetPos, 8.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
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

	if (Stage::ColCheckGround(target, cameraPos + lay, &hitPos)) {
		Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * Vector3::UnitY;	// 地面との設置点
		Vector3 targePos = target * Vector3::UnitY;

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
	
    target = transform->position + CAMERADEFINE_REF.m_TargetDef * transform->RotationMatrix();

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

void Camera::UpdateOffsetLeap() {

	offset += ((offsetAfter - offset) * CAMERADEFINE_REF.m_Interpolation) * GTime.timeScale;
}

void Camera::UpdateTargetLeap() {

	target += ((targetAfter - target) * CAMERADEFINE_REF.m_Interpolation) * GTime.timeScale;
}

void Camera::UpdateAnimation() {

	if (m_AnimationSec > 0.0f) {
		m_AnimationSec -= GTime.deltaTime;

		const float TimeHeaf = m_AnimationSecMax * 0.5f;

		if (m_AnimationSec > TimeHeaf)
			offset = EasingFunc::Linear(m_AnimationSec - TimeHeaf, TimeHeaf, m_AnimationBegin, m_AnimationEnd);
		else
			offset = EasingFunc::Linear(m_AnimationSec, TimeHeaf, m_AnimationEnd, m_AnimationBegin);
	}
	else {
		m_AnimationSec = 0.0f;
	}
}

void Camera::SetPerformance(const std::string& perfType) {

	//stateManager->ChangeState(State::sPerformance);
	//dynamic_cast<CameraState_Performance*>(stateManager->State(State::sPerformance))->SetCameraWork(perfType);
}

void Camera::SetAnimation(const Vector3& begin, const Vector3& end, const Vector3& target, float sec) {

	m_AnimationBegin = begin;
	m_AnimationEnd = end;
	m_AnimationTarget = target;
	m_AnimationSec= duration;
	m_AnimationSecMax= duration;
}

Vector3 Camera::WorldPos() const {

	Vector3 globalPos = transform->Global().position;	// カメラの絶対座標
	Vector3 offsetRotAdap = OffsetRotAdaptor();			// 回転行列をかけた相対座標
	Vector3 holderPos = Vector3::Zero;	// 保有者の座標

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
