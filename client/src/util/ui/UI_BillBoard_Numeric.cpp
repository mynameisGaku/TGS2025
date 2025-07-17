#include "src/util/ui/UI_BillBoard_Numeric.h"
#include "ui_Manager.h"

#include "src/common/camera/cameraManager.h"
#include <src/util/ptr/PtrUtil.h>

UI_BillBorad_Numeric::UI_BillBorad_Numeric() {

	clipSize = 0.0f;
}

UI_BillBorad_Numeric::~UI_BillBorad_Numeric() {

	PtrUtil::SafeDeleteVector(numbers);
}

void UI_BillBorad_Numeric::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	Camera* camera = CameraManager::MainCamera();
	if (camera == nullptr)
		return;

	Vector3 offset = Vector3::Zero;	// 桁数ごとに描画位置をズラす為の相対座標
	int index = 0;				// 要素数

	for (auto itr = numbers.begin(); itr != numbers.end();) {

		UI_BillBoard* bill = numbers[index];
		Vector3 defPos = numbersPos[index];

		// 桁数ごとに描画位置をズラす
		bill->transform->position = defPos + offset;

		bill->Update();

		// カメラから見て、左側に進める
		offset += camera->transform->Right() * (clipSize - 5.0f) * -1.0f;

		// イージング挙動が稼働しているか
		if ((*itr)->IsAllEasingRun() == false) {
			delete (*itr);
			itr = numbers.erase(itr);
		}
		else {
			itr++;
			index++;
		}
	}
}

void UI_BillBorad_Numeric::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	for (const auto& itr : numbers)
		itr->Draw();
}

void UI_BillBorad_Numeric::Add(const int numberGraph[10], const Transform& trs, const Vector2& center, const float& _clipSize, const int& value) {

	int digit = 1;	// 現在の桁数
	int digitMax = MathUtil::Digit(value);	// 桁数

	clipSize = _clipSize;

	// 数字に応じた画像をビルボードに設定する
	for (int i = 0; i < digitMax; i++) {

		// 数値に応じた画像
		int graph = numberGraph[(value / digit) % 10];

		UI_BillBoard* billBoard = new UI_BillBoard(graph, trs, center, clipSize);
		billBoard->SetEasing(UI_Canvas::eAlpha, 255, 0, 1.25f, EasingType::InCubic);

		UI_Manager::Detach(billBoard);

		numbers.push_back(billBoard);
		numbersPos.push_back(trs.position);

		digit *= 10;
	}
}
