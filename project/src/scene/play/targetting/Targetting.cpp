#include "Targetting.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/setting/window/WindowSetting.h"

Targetting::Targetting() {

	ballManager = nullptr;
	charaManager = nullptr;
	targetList.clear();

	hArrow = LoadGraph("data/texture/arrow.png");
}

Targetting::~Targetting() {

	targetList.clear();

	DeleteGraph(hArrow);
}

void Targetting::Start() {

	ballManager = FindGameObject<BallManager>();
	charaManager = FindGameObject<CharaManager>();
}

void Targetting::Update() {

	int cameraNum = CameraManager::AllCameras().size();

	for (int i = 0; i < cameraNum; i++) {

		Camera* camera = CameraManager::GetCamera(i);
		if (camera->TargetChara() == nullptr) {
			targetList[i] = -1;
			continue;
		}

		targetList[i] = camera->TargetChara()->GetIndex();
	}
}

void Targetting::Draw() {

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = CameraManager::GetScreenDivisionCenter();
	DrawCircleAA(screenCenter.x, screenCenter.y, circleRadius, 16, GetColor(255, 0, 0), false, 2.0f);

	for (auto& target : targetList) {

		int index = target.first;
		int targetIndex = target.second;

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

		// 狙われているキャラ
		CharaBase* charaTarget = charaPool->Get(targetIndex);
		if (charaTarget == nullptr)
			continue;


		Camera* targetCamera = CameraManager::GetCamera(targetIndex);
		if (targetCamera == nullptr)
			continue;

		if (not targetCamera->IsDrawEnd())
			continue;

		// ボールを所持している場合
		Ball* ball = chara->GetHaveBall();
		if (ball != nullptr && chara->IsCharging()) {

			// 距離
			Vector3 dir = ball->transform->Global().position - charaTarget->transform->Global().position;

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

			SetDrawBright(255, 55 + 200 * distNormalize, 55 + 200 * distNormalize);

			Vector2 markerFromCenter = markerPos - screenCenter;
			float arrowAngle = atan2f(markerFromCenter.y, markerFromCenter.x) + DX_PI_F / 2.0f;

			DrawRectRotaGraphF(markerPos.x, markerPos.y, 0, 0, 32, 32, 2.0f, arrowAngle, hArrow, true, false);

			SetDrawBright(255, 255, 255);
		}

		const Ball* targetBall = chara->LastBall();
		if (targetBall != nullptr && targetBall->GetState() == Ball::State::S_THROWN) {
		
			// 距離
			Vector3 dir = targetBall->transform->Global().position - charaTarget->transform->Global().position;

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

			SetDrawBright(255, 255 * distNormalize, 255 * distNormalize);

			Vector2 markerFromCenter = markerPos - screenCenter;
			float arrowAngle = atan2f(markerFromCenter.y, markerFromCenter.x) + DX_PI_F / 2.0f;

			DrawRectRotaGraphF(markerPos.x, markerPos.y, 0, 0, 32, 32, 2.0f, arrowAngle, hArrow, true, false);

			SetDrawBright(255, 255, 255);
		}
	}
}

int Targetting::IsTargetting(int myIndex) {

	if (targetList.contains(myIndex))
		return -1;

	return targetList[myIndex];
}
