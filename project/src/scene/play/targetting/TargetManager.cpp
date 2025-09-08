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

#include "src/scene/play/ui/UI_Target.h"
#include "src/util/ui/UI_Manager.h"

TargetManager::TargetManager()
{
	ballManager = nullptr;
	charaManager = nullptr;

	//m_UI_Target = Instantiate<UI_Target>();
	m_UI_Target = new UI_Target();
	m_UI_Target->SetTargetManager(this);
	//SceneManager::CurrentScene()->SetDrawOrder(m_UI_Target, 10000);
}

TargetManager::~TargetManager()
{
}

void TargetManager::Start()
{
	ballManager = FindGameObject<BallManager>();
	charaManager = FindGameObject<CharaManager>();
}

void TargetManager::Update()
{
	m_UI_Target->SetTargetManager(this);
}

void TargetManager::Draw()
{
	m_UI_Target->SetTargetManager(this);
}

void TargetManager::UpdateUI()
{
}

void TargetManager::DrawUI()
{
	// 現在描画を行っているカメラID
	int cameraID = CameraManager::GetCurrentDrawingCameraID();
	Vector2 pos = CameraManager::GetDrawingAreaPos_CameraIndex(cameraID);
	Vector2 size = Vector2(WindowSetting::Inst().width, WindowSetting::Inst().height);
	Vector2 clipSize = CameraManager::GetDrawingAreaSize_CameraIndex(cameraID);

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
		markerRect.anchor.SetEnd(pos + clipSize);
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

	//const Vector2 screenCenter = pos + size * 0.5f;
	const Vector2 screenCenter = pos + clipSize * 0.5f;

	// 外周の赤い警告表示
	Vector2 begin = screenCenter - clipSize * 0.5f;
	Vector2 end = screenCenter + clipSize * 0.5f;
	DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(255, 0, 0), false, 10.0f);

	// 狙われているボールの位置への棘表示
	for (const auto& item : charaRockOnData)
	{
		const RockOnData data = item.second;
		const Ball* ball = ballManager->GetBall(data.BallIndex);

		const float circleRadius = 32.0f;

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

		Vector2 ballPos = Vector2(drawPoint.x, drawPoint.z).Normalize() * clipSize;
		ballPos.x = std::clamp(ballPos.x, clipSize.x * -0.5f, clipSize.x * 0.5f);
		ballPos.y = std::clamp(ballPos.y, clipSize.y * -0.5f, clipSize.y * 0.5f);

		float thornAngle = atan2f(ballPos.y, ballPos.x) + DX_PI_F / 2.0f * -1.0f;

		Vector2 thornBackOffset1 = Vector2(30.0f, 0.0f) * cosf(thornAngle);
		Vector2 thornBackOffset2 = Vector2(0.0f, 30.0f) * -sinf(thornAngle);
		Vector2 thornBackOffset3 = ballPos * 0.4f;

		Vector2 thornBackPos1 = screenCenter + thornBackOffset1 + ballPos;
		Vector2 thornBackPos2 = screenCenter + thornBackOffset2 + ballPos;
		Vector2 thornBackPos3 = screenCenter + thornBackOffset3;

		DrawTriangleAA(thornBackPos1.x, thornBackPos1.y, thornBackPos2.x, thornBackPos2.y, thornBackPos3.x, thornBackPos3.y, 0x00000, true, -1.0f);


		Vector2 thornOffset1 = Vector2(30.0f, 0.0f) * cosf(thornAngle);
		Vector2 thornOffset2 = Vector2(0.0f, 30.0f) * -sinf(thornAngle);
		Vector2 thornOffset3 = ballPos * 0.5f;

		Vector2 thornPos1 = screenCenter + thornOffset1 + ballPos;
		Vector2 thornPos2 = screenCenter + thornOffset2 + ballPos;
		Vector2 thornPos3 = screenCenter + thornOffset3;

		DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornPos3.x, thornPos3.y, 0xFFFFFF, true, -1.0f);
	}
}

void TargetManager::DrawThorn() {

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

	const BallTarget* charaTarget = chara->GetBallTarget();
	const std::unordered_map<int, RockOnData> charaRockOnData = charaTarget->GetRockOnData();

	if (charaRockOnData.empty()) return;

	// 外周の赤い警告表示
	DrawBoxAA(size.x * 0.5f, 0.0f, size.x * 1.5f, size.y, GetColor(255, 0, 0), false, 10.0f);

	int index = 0;

	// 狙われているボールの位置への棘表示
	for (const auto& item : charaRockOnData)
	{
		const RockOnData data = item.second;
		const Ball* ball = ballManager->GetBall(data.BallIndex);

		const float circleRadius = 32.0f;
		const Vector2 screenCenter = pos + size * 0.5f;

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

		Vector2 thornPos = screenCenter + Vector2(drawPoint.x, drawPoint.z).Normalize() * screenDivSize;

		Vector2 thornFromCenter = thornPos - screenCenter;
		float thornAngle = atan2f(thornFromCenter.y, thornFromCenter.x) + DX_PI_F / 2.0f * -1.0f;

		Vector2 thornOffset1 = Vector2(30.0f, 0.0f) * cosf(thornAngle);
		Vector2 thornOffset2 = Vector2(0.0f, 30.0f) * -sinf(thornAngle);
		Vector2 thornOffset3 = Vector2(drawPoint.x, drawPoint.z) * screenDivSize * 0.35f;

		Vector2 thornPos1 = thornPos + thornOffset1;
		Vector2 thornPos2 = thornPos + thornOffset2;
		Vector2 thornPos3 = screenCenter + thornOffset3;
		Vector2 thornBackPos3 = screenCenter + Vector2(drawPoint.x, drawPoint.z) * screenDivSize * 0.3f;

		DrawTriangleAA(thornPos1.x, thornPos1.y, thornPos2.x, thornPos2.y, thornPos3.x, thornPos3.y, 0xFFFFFF, true, 10.0f);

		//if (m_UI_Target.size() <= index)
		//{
		//	UI_Target* target = new UI_Target();
		//	target->SetArrowGraph("data/texture/arrow.png");
		//	target->SetCamIndex(cameraID);
		//	target->SetTargetPoint(thornPos1, thornPos2, thornPos3, GetColor(255, 255, 255), 10.0f);
		//	m_UI_Target.push_back(target);
		//}

		//if (m_UI_Target[index])
		//{
		//	m_UI_Target[index]->SetIsActive(true);
		//	m_UI_Target[index]->SetTargetPoint(thornPos1, thornPos2, thornPos3, GetColor(255, 255, 255), 10.0f);
		//}

		//index++;
	}
}
