#include "camera.h"
#include "cameraManager.h"

// ◇汎用
#include "../Library/myDxLib.h"
#include "../Library/time.h"
#include "../Source/Util/Utils.h"
#include "../Source/Stage.h"
#include <assert.h>

// ◇ステート関連
#include "StateManager.h"

// ◇コンポーネント
#include "Component/Shake.h"

// ◇個別で必要な物
#include "InputManager.h"
#include "PadController.h"
#include "MouseController.h"
#include "CharaManager.h"

using namespace KeyDefine;
using namespace CameraDefine;

Camera::Camera() {

	Reset();

	cameraWork = nullptr;
	
	AddComponent<Shake>();

	// fsmの初期化
	fsm = new TinyFSM<Camera>(this);
    fsm->RegisterStateName(&Camera::DebugState, "DebugState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
    //fsm->RegisterStateName(&Camera::ChaseState, "ChaseState"); // この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	fsm->ChangeState(&Camera::DebugState); // ステートを変更
	
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


	MATRIX mShakeTrs = MGetIdent();	// 振動用行列

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
		Vector3 terrainPos = (hitPos - OffsetRotAdaptor()) * V3::UP;	// 地面との設置点
		Vector3 targePos = target * V3::HORIZONTAL;

		transform->position = terrainPos + targePos + V3::SetY(10.0f);
	}
}

void Camera::MoveProcess()
{
	//====================================================================================================
	// ▼マウスによるカメラの向き変更

	transform->rotation.x += (MouseController::Info().Move().y * Math::DegToRad(1.0f));
	transform->rotation.y += (MouseController::Info().Move().x * Math::DegToRad(1.0f));

	// X軸角度の制限
	transform->rotation.x = min(max(transform->rotation.x, CAMERA_ROT_X_MIN), CAMERA_ROT_X_MAX);
	
    target = transform->position + CAMERA_TARGET_DEF * transform->RotationMatrix();

	//====================================================================================================
	// ▼移動処理

	Vector3 velocity = (InputManager::AnalogStick() * MOVE_SPEED) * transform->RotationMatrix();

	if (InputManager::Hold(KeyCode::Space))
		velocity.y += SHIFT_SPEED;

	else if (InputManager::Hold(KeyCode::LeftShift))
		velocity.y -= SHIFT_SPEED;

	transform->position += velocity;
}

void Camera::OperationByMouse() {

	Vector2 addRot = V2::ZERO;	// 加算する回転量

	MouseController::MouseInfo mouse = MouseController::Info();	// マウスの情報

	addRot.x = (mouse.moveY * mouse.sensitivity.y) * Math::DegToRad(0.1f);
	addRot.y = (mouse.moveX * mouse.sensitivity.x) * Math::DegToRad(0.1f);

	// 勢いがつき過ぎない様に、制限をかける
	transform->rotation.x += min(max(addRot.x, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);
	transform->rotation.y += min(max(addRot.y, -ROT_SPEED_LIMIT), ROT_SPEED_LIMIT);
}

void Camera::OperationByStick() {

	Vector2 addRot = V2::ZERO;	// 加算する回転量
	Vector2 rightStick = PadController::NormalizedRightStick();

	if (rightStick.SquareSize() < KeyDefine::STICK_DEADZONE)
		return;

	addRot.x = (rightStick.y * PadController::StickSensitivity().y) * Math::DegToRad(-1.0f);
	addRot.y = (rightStick.x * PadController::StickSensitivity().x) * Math::DegToRad(1.0f);

	// 勢いがつき過ぎない様に、制限をかける
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

	Vector3 globalPos = transform->Global().position;	// カメラの絶対座標
	Vector3 offsetRotAdap = OffsetRotAdaptor();			// 回転行列をかけた相対座標
	Vector3 holderPos = V3::ZERO;	// 保有者の座標

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

///////////////////////////////////////////////////////////////////////////
// 
//			▼ステート
//

void Camera::ChaseState(FSMSignal sig)
{
	// 移動可能か
	static bool canMove;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
		canMove = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
		OperationByMouse();
		OperationByStick();

		CharaManager* charaM = FindGameObject<CharaManager>();
		if (charaM == nullptr)
			return;

		const CharaBase* chara = charaM->CharaInst(m_CharaIndex);
		if (chara == nullptr)
			return;

		const Transform charaTrs = chara->transform->Global();

		SetOffset(CAMERA_OFFSET_CHASE);
		SetTarget(Vector3(0.0f, 100.0f, 0.0f) * charaTrs.Matrix());
		transform->position = charaTrs.position;

		ColCheckToTerrain();

		transform->rotation.x = Math::Clamp(transform->rotation.x, CAMERA_ROT_X_MIN, CAMERA_ROT_X_MAX);

		if (transform->rotation.y < -Math::PI)
			transform->rotation.y += Math::PI_TW;
		else if (transform->rotation.y > Math::PI)
			transform->rotation.y -= Math::PI_TW;
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{

	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
		canMove = true;
	}
	break;
	}
}

void Camera::DebugState(FSMSignal sig)
{
	// 移動可能か
	static bool canMove;

	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 初期化 (Constractor)
	{
		canMove = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新 (Update)
	{
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
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新 (AfterUpdate)
	{

	}
	break;
	case FSMSignal::SIG_Exit: // 終了 (Exit)
	{
		canMove = true;
	}
	break;
	}
}