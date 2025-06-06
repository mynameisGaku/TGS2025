#include "TargetManager.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/setting/window/WindowSetting.h"

TargetManager::TargetManager() {

	ballManager = nullptr;
	charaManager = nullptr;
	checkCamera.clear();
	targetList.clear();

	hArrow = LoadGraph("data/texture/arrow.png");
}

TargetManager::~TargetManager() {

	checkCamera.clear();
	targetList.clear();

	DeleteGraph(hArrow);
}

void TargetManager::Start() {

	ballManager = FindGameObject<BallManager>();
	charaManager = FindGameObject<CharaManager>();

	const int cameraNum = (int)CameraManager::AllCameras().size();
	for (int i = 0; i < cameraNum; i++)
		checkCamera.push_back(false);
}

void TargetManager::Update() {

	// カメラの総数
	const int cameraNum = CameraManager::AllCameras().size();

	for (int i = 0; i < cameraNum; i++) {
		Camera* camera = CameraManager::GetCamera(i);

		// カメラが誰も注視していない場合
		if (camera->TargetChara() == nullptr) {
			targetList[i] = -1;
			continue;
		}

		// 注視していたキャラの番号を取得
		targetList[i] = camera->TargetChara()->GetIndex();
	}

	// チェック済マークのリセット
	for (auto check : checkCamera)
		check = false;
}

void TargetManager::Draw() {

	// カメラの総数
	const int cameraNum = (int)CameraManager::AllCameras().size();

	for (int i = 0; i < cameraNum; i++) {

		Camera* camera = CameraManager::GetCamera(i);
		if (camera == nullptr)
			continue;

		// カメラの描画が完了しているかつ、チェック済の場合
		if (camera->IsDrawEnd() && checkCamera[i])
			continue;

		int index = i;					// カメラの番号＆キャラの番号
		int targetIndex = targetList[i];// 注視しているキャラの番号

		// ターゲットが居ない場合
		if (targetIndex == -1)
			continue;

		Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
		if (charaPool == nullptr)
			continue;

		// 狙っているキャラ
		CharaBase* chara = charaPool->Get(index);
		if (chara == nullptr)
			continue;

		// 狙っているキャラに対応するカメラを取得
		Camera* targetCamera = CameraManager::GetCamera(targetIndex);
		if (targetCamera == nullptr)
			continue;

		// 描画が完了していない場合
		// 警告マーカーを描画するのは相手方のカメラなので、相手方の描画が終わるのを待つ
		if (not targetCamera->IsDrawEnd())
			continue;

		const Ball* haveBall = chara->GetHaveBall();
		const Ball* lastBall = chara->LastBall();

		// ボールをチャージしている場合
		if (haveBall != nullptr && chara->IsCharging()) {
			DrawWarning();
			DrawThorn(haveBall->transform->Global().position, targetIndex);
		}

		// ボールを投げた場合
		if (lastBall != nullptr && lastBall->GetState() == Ball::State::S_THROWN &&
			lastBall->GetLastOwner() == chara) {
			DrawWarning();
			DrawThorn(lastBall->transform->Global().position, targetIndex);
		}

		checkCamera[i] = true;
	}
}

int TargetManager::TargetID(int charaIndex) {

	if (targetList.contains(charaIndex))
		return -1;

	return targetList[charaIndex];
}

void TargetManager::DrawBallPosMarker(const Vector3& ballPos, int targetCharaID) {

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = CameraManager::GetScreenDivisionCenter();
	DrawCircleAA(screenCenter.x, screenCenter.y, circleRadius, 16, GetColor(255, 0, 0), false, 2.0f);

	Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// 狙われているキャラ
	CharaBase* charaTarget = charaPool->Get(targetCharaID);
	if (charaTarget == nullptr)
		return;

	// 狙っているキャラに対応するカメラを取得
	Camera* targetCamera = CameraManager::GetCamera(targetCharaID);
	if (targetCamera == nullptr)
		return;

	if (not targetCamera->IsDrawEnd())
		return;

	// 距離
	Vector3 dir = ballPos - charaTarget->transform->Global().position;

	// 距離を長さに変換
	float distance = dir.GetLength();

	// 角度の計算
	float angle = atan2f(dir.z, dir.x);

	// Z-X平面で考える
	float dz = -sinf(angle);
	float dx = cosf(angle);

	// 描画位置
	Vector3 drawPoint = Vector3(dx, 0.0f, dz);

	// 描画位置をビュー行列で回す
	drawPoint = VTransform(drawPoint, targetCamera->transform->RotationMatrix());

	// 画面中央のサークル
	Vector2 markerPos = screenCenter + Vector2(drawPoint.x, drawPoint.z) * circleRadius;

	// マーカーの大きさを距離に応じて変える
	const float DistMax = 6000.0f;

	// 距離が最大値を超えた場合は最大値にする
	MathUtil::ClampAssing(&distance, 0.0f, DistMax);

	// 正規化された距離を計算
	float distNormalize = distance / DistMax;

	Vector2 markerFromCenter = markerPos - screenCenter;
	float arrowAngle = atan2f(markerFromCenter.y, markerFromCenter.x) + DX_PI_F / 2.0f;

	SetDrawBright(180 + 75 * distNormalize, 0, 0);

	DrawRectRotaGraphF(markerPos.x, markerPos.y, 0, 0, 32, 32, 2.0f, arrowAngle, hArrow, true, false);

	SetDrawBright(255, 255, 255);
}

void TargetManager::DrawWarning() {

	Vector2 scrPos = CameraManager::GetScreenDivisionPos();
	Vector2 scrSize = CameraManager::GetScreenDivisionSize();

	DrawBoxAA(scrPos.x + 1.0f, scrPos.y + 1.0f, scrPos.x + scrSize.x - 1.0f, scrPos.y + scrSize.y - 1.0f, GetColor(255, 0, 0), false, 10.0f);
}

void TargetManager::DrawThorn(const Vector3& ballPos, int targetCharaID) {

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = CameraManager::GetScreenDivisionCenter();
	
	Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// 狙われているキャラ
	CharaBase* charaTarget = charaPool->Get(targetCharaID);
	if (charaTarget == nullptr)
		return;

	// 狙っているキャラに対応するカメラを取得
	Camera* targetCamera = CameraManager::GetCamera(targetCharaID);
	if (targetCamera == nullptr)
		return;

	if (not targetCamera->IsDrawEnd())
		return;

	Vector2 screenDivSize = CameraManager::GetScreenDivisionSize();

	// 距離
	Vector3 dir = ballPos - charaTarget->transform->Global().position;

	// 距離を長さに変換
	float distance = dir.GetLength();

	// 角度の計算
	float angle = atan2f(dir.z, dir.x);

	// Z-X平面で考える
	float dz = -sinf(angle);
	float dx = cosf(angle);

	// 描画位置
	Vector3 drawPoint = Vector3(dx, 0.0f, dz);

	// 描画位置をビュー行列で回す
	drawPoint = VTransform(drawPoint, targetCamera->transform->RotationMatrix());

	Vector2 thornPos = screenCenter + Vector2(drawPoint.x, drawPoint.z) * screenDivSize;

	Vector2 thornFromCenter = thornPos - screenCenter;
	float thornAngle = atan2f(thornFromCenter.y, thornFromCenter.x) + DX_PI_F / 2.0f * -1.0f;

	Vector2 thornOffset1 = Vector2(50.0f, 0.0f) * cosf(thornAngle);
	Vector2 thornOffset2 = Vector2(0.0f, 50.0f) * -sinf(thornAngle);
	Vector2 thornOffset3 = Vector2(drawPoint.x, drawPoint.z) * screenDivSize * 0.35f;

	Vector2 thornPos1 = thornPos + thornOffset1;
	Vector2 thornPos2 = thornPos + thornOffset2;
	Vector2 thornPos3 = screenCenter + thornOffset3;
	Vector2 thornBackPos3 = screenCenter + Vector2(drawPoint.x, drawPoint.z) * screenDivSize * 0.3f;

	DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornBackPos3.x, thornBackPos3.y, GetColor(0,0,0), true, 10.0f);
	DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornPos3.x, thornPos3.y, GetColor(255,255,255), true, 10.0f);
}
