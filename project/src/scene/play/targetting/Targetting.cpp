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

	for (auto& target : targetList) {

		int index = target.first;
		int targetIndex = target.second;

		// �^�[�Q�b�g�����Ȃ��ꍇ
		if (targetIndex == -1)
			continue;

		Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
		if (charaPool == nullptr)
			continue;

		// �_���Ă���L����
		CharaBase* chara = charaPool->Get(index);
		if (chara == nullptr)
			continue;

		// �_���Ă���L�����ɑΉ�����J�������擾
		Camera* targetCamera = CameraManager::GetCamera(targetIndex);
		if (targetCamera == nullptr)
			return;

		if (not targetCamera->IsDrawEnd())
			return;

		// �{�[�����`���[�W���Ă���ꍇ
		Ball* ball = chara->GetHaveBall();
		if (ball != nullptr && chara->IsCharging()) {
			DrawBallPosMarker(ball->transform->Global().position, targetIndex);
			DrawWarning();
			DrawThorn(ball->transform->Global().position, targetIndex);
		}

		// �{�[���𓊂����ꍇ
		const Ball* targetBall = chara->LastBall();
		if (targetBall != nullptr && targetBall->GetState() == Ball::State::S_THROWN) {
			DrawBallPosMarker(targetBall->transform->Global().position, targetIndex);
			DrawWarning();
			DrawThorn(targetBall->transform->Global().position, targetIndex);
		}
	}
}

int Targetting::IsTargetting(int myIndex) {

	if (targetList.contains(myIndex))
		return -1;

	return targetList[myIndex];
}

void Targetting::DrawBallPosMarker(const Vector3& ballPos, int targetCharaID) {

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = CameraManager::GetScreenDivisionCenter();
	DrawCircleAA(screenCenter.x, screenCenter.y, circleRadius, 16, GetColor(255, 0, 0), false, 2.0f);

	Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// �_���Ă���L����
	CharaBase* charaTarget = charaPool->Get(targetCharaID);
	if (charaTarget == nullptr)
		return;

	// �_���Ă���L�����ɑΉ�����J�������擾
	Camera* targetCamera = CameraManager::GetCamera(targetCharaID);
	if (targetCamera == nullptr)
		return;

	if (not targetCamera->IsDrawEnd())
		return;

	// ����
	Vector3 dir = ballPos - charaTarget->transform->Global().position;

	// �����𒷂��ɕϊ�
	float distance = dir.GetLength();

	// �p�x�̌v�Z
	float angle = atan2f(dir.z, dir.x);

	// Z-X���ʂōl����
	float dz = -sinf(angle);
	float dx = cosf(angle);

	// �`��ʒu
	Vector3 drawPoint = Vector3(dx, 0.0f, dz);

	// �`��ʒu���r���[�s��ŉ�
	drawPoint = VTransform(drawPoint, targetCamera->transform->RotationMatrix());

	// ��ʒ����̃T�[�N��
	Vector2 markerPos = screenCenter + Vector2(drawPoint.x, drawPoint.z) * circleRadius;

	// �}�[�J�[�̑傫���������ɉ����ĕς���
	const float DistMax = 6000.0f;

	// �������ő�l�𒴂����ꍇ�͍ő�l�ɂ���
	MathUtil::ClampAssing(&distance, 0.0f, DistMax);

	// ���K�����ꂽ�������v�Z
	float distNormalize = distance / DistMax;

	Vector2 markerFromCenter = markerPos - screenCenter;
	float arrowAngle = atan2f(markerFromCenter.y, markerFromCenter.x) + DX_PI_F / 2.0f;

	SetDrawBright(180 + 75 * distNormalize, 0, 0);

	DrawRectRotaGraphF(markerPos.x, markerPos.y, 0, 0, 32, 32, 2.0f, arrowAngle, hArrow, true, false);

	SetDrawBright(255, 255, 255);
}

void Targetting::DrawWarning() {

	Vector2 scrPos = CameraManager::GetScreenDivisionPos();
	Vector2 scrSize = CameraManager::GetScreenDivisionSize();

	DrawBoxAA(scrPos.x + 1.0f, scrPos.y + 1.0f, scrPos.x + scrSize.x - 1.0f, scrPos.y + scrSize.y - 1.0f, GetColor(255, 0, 0), false, 10.0f);
}

void Targetting::DrawThorn(const Vector3& ballPos, int targetCharaID) {

	const float circleRadius = 32.0f;
	const Vector2 screenCenter = CameraManager::GetScreenDivisionCenter();
	DrawCircleAA(screenCenter.x, screenCenter.y, circleRadius, 16, GetColor(255, 0, 0), false, 2.0f);

	Pool<CharaBase>* charaPool = charaManager->GetCharaPool();
	if (charaPool == nullptr)
		return;

	// �_���Ă���L����
	CharaBase* charaTarget = charaPool->Get(targetCharaID);
	if (charaTarget == nullptr)
		return;

	// �_���Ă���L�����ɑΉ�����J�������擾
	Camera* targetCamera = CameraManager::GetCamera(targetCharaID);
	if (targetCamera == nullptr)
		return;

	if (not targetCamera->IsDrawEnd())
		return;

	Vector2 screenDivSize = CameraManager::GetScreenDivisionSize();

	// ����
	Vector3 dir = ballPos - charaTarget->transform->Global().position;

	// �����𒷂��ɕϊ�
	float distance = dir.GetLength();

	// �p�x�̌v�Z
	float angle = atan2f(dir.z, dir.x);

	// Z-X���ʂōl����
	float dz = -sinf(angle);
	float dx = cosf(angle);

	// �`��ʒu
	Vector3 drawPoint = Vector3(dx, 0.0f, dz);

	// �`��ʒu���r���[�s��ŉ�
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
