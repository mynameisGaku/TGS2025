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

#include "CameraState_Debug.h"

// ◇コンポーネント
#include "Component/Shake.h"

using namespace CameraDefine;

Camera::Camera() {

	Reset();

	stateManager = nullptr;
	cameraWork = nullptr;
	
	AddComponent<Shake>();

	stateManager = new StateManager(this);
	stateManager->AddInstance(State::sDebug, new CameraState_Debug(stateManager));

	stateManager->ChangeState(State::sDebug);

	//cameraWork = new CsvReader("data/csv/CameraWork.csv");
}

Camera::~Camera() {

	holder = nullptr;
	Function::DeletePointer(stateManager);
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

	if (stateManager != nullptr)
		stateManager->Update();

	Object3D::Update();
}

void Camera::Draw() {

	Object3D::Draw();

	if (stateManager != nullptr)
		stateManager->Draw();

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

void Camera::ChangeState(const CameraDefine::State& id) {

	if (id >= State::sMax)
		assert(0 && "CameraStateを指定出来ません");

	stateManager->ChangeState(id);
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

void Camera::SetPerformance(const std::string& perfType) {

	stateManager->ChangeState(State::sPerformance);
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
