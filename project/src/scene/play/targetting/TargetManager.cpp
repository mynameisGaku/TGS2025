#include "TargetManager.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/camera/CameraManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/fx/effect/EffectManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/screen/ScreenManager.h"
#include "src/scene/play/ball/BallTarget.h"

TargetManager::TargetManager()
{
	ballManager = nullptr;
	charaManager = nullptr;

	hArrow = LoadGraph("data/texture/arrow.png");
}

TargetManager::~TargetManager()
{
	DeleteGraph(hArrow);
}

void TargetManager::Start()
{
	ballManager = FindGameObject<BallManager>();
	charaManager = FindGameObject<CharaManager>();
}

void TargetManager::Update()
{

}

void TargetManager::Draw()
{
	// 現在描画を行っているカメラID
	int cameraID = CameraManager::GetCurrentDrawingCameraID();
	Vector2 pos = CameraManager::GetDrawingAreaPos_CameraIndex(cameraID);
	Vector2 size = CameraManager::GetDrawingAreaSize_CameraIndex(cameraID);

	Camera* camera = CameraManager::GetCamera(cameraID);
	if (camera == nullptr)
		return;

	Pool<Chara>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// カメラに対応するキャラ
	Chara* chara = charaPool->Get(cameraID);
	if (chara == nullptr)
		return;

	// カメラが注視しているターゲット
	BallTarget* cameraTarget = camera->GetBallTarget();

	// ボールをチャージしている場合
	if (chara->IsCharging() && cameraTarget)
	{

#if 0
		// 狙っているターゲットをスクリーン座標にしてマーカー表示
		Vector3 scPosition = ConvWorldPosToScreenPos(cameraTarget->Position());

		RectTransform markerRect = RectTransform(Anchor::Preset::LeftUp, Vector2(scPosition.x, scPosition.y));
		Vector2 beginPos = ScreenManager::GetScreenBeginPos(cameraID);
		Vector2 endPos = ScreenManager::GetScreenEndPos(cameraID);
		markerRect.anchor.SetBegin(beginPos);
		markerRect.anchor.SetEnd(endPos);
#else
		// マーカーは画面中央固定
		RectTransform markerRect = RectTransform(Anchor::Preset::Middle);
		markerRect.anchor.SetBegin(pos);
		markerRect.anchor.SetEnd(pos + size);
#endif // 0

		EffectBase* lockOn = EffectManager::Play2D_Loop("LockOnMarker_001.efk", markerRect, StringUtil::FormatToString("LockOn %d", cameraID));

		lockOn->SetPlaySpeed(2.0f - chara->GetBallChargeRate() * 2.0f + 0.1f);
		lockOn->SetScale2D(1.0f - chara->GetBallChargeRate() * 0.25f);
	}
	else
	{
		EffectManager::Stop("LockOnMarker_001.efk", StringUtil::FormatToString("LockOn %d", cameraID));
	}

	if (not ballManager) return;

	const BallTarget* charaTarget = chara->GetBallTarget();
	const std::unordered_map<int, RockOnData> charaRockOnData = charaTarget->GetRockOnData();

	if (charaRockOnData.empty()) return;

	// 外周の赤い警告表示
	DrawBoxAA(size.x * 0.5f, 0.0f, size.x * 1.5f, size.y, GetColor(255, 0, 0), false, 10.0f);

	// 狙われているボールの位置への棘表示
	for (const auto& item : charaRockOnData)
	{
		const RockOnData data = item.second;
		const Ball* ball = ballManager->GetBall(data.BallIndex);

		const float circleRadius = 32.0f;
		const Vector2 screenCenter = Vector2(size.x , size.y * 0.75f);

		Vector2 screenDivSize = size;

		// 距離
		Vector3 dir = ball->transform->Global().position - chara->transform->Global().position;

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
		drawPoint = VTransform(drawPoint, camera->transform->RotationMatrix());

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

		DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornBackPos3.x, thornBackPos3.y, GetColor(0, 0, 0), true, 10.0f);
		DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornPos3.x, thornPos3.y, GetColor(255, 255, 255), true, 10.0f);
	}
}

void TargetManager::DrawBallPosMarker(const Vector3& ballPos, int targetCharaID) {

	if (CameraManager::GetCurrentDrawingCameraID() != targetCharaID)
		return;

	Vector2 pos = CameraManager::GetDrawingAreaPos_CameraIndex(targetCharaID);
	Vector2 size = CameraManager::GetDrawingAreaSize_CameraIndex(targetCharaID);

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = pos + size * 0.5f;
	DrawCircleAA(screenCenter.x, screenCenter.y, circleRadius, 16, GetColor(255, 0, 0), false, 2.0f);

	Pool<Chara>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// 狙われているキャラ
	Chara* charaTarget = charaPool->Get(targetCharaID);
	if (charaTarget == nullptr)
		return;

	// 狙っているキャラに対応するカメラを取得
	Camera* targetCamera = CameraManager::GetCamera(targetCharaID);
	if (targetCamera == nullptr)
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

	SetDrawBright(180 + (int)(75.0f * distNormalize), 0, 0);

	DrawRectRotaGraphF(markerPos.x, markerPos.y, 0, 0, 32, 32, 2.0f, arrowAngle, hArrow, true, false);

	SetDrawBright(255, 255, 255);
}
