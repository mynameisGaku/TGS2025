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

	Vector3 offset = Vector3::Zero;	// �������Ƃɕ`��ʒu���Y�����ׂ̑��΍��W
	int index = 0;				// �v�f��

	for (auto itr = numbers.begin(); itr != numbers.end();) {

		UI_BillBoard* bill = numbers[index];
		Vector3 defPos = numbersPos[index];

		// �������Ƃɕ`��ʒu���Y����
		bill->transform->position = defPos + offset;

		bill->Update();

		// �J�������猩�āA�����ɐi�߂�
		offset += camera->transform->Right() * (clipSize - 5.0f) * -1.0f;

		// �C�[�W���O�������ғ����Ă��邩
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

	int digit = 1;	// ���݂̌���
	int digitMax = MathUtil::Digit(value);	// ����

	clipSize = _clipSize;

	// �����ɉ������摜���r���{�[�h�ɐݒ肷��
	for (int i = 0; i < digitMax; i++) {

		// ���l�ɉ������摜
		int graph = numberGraph[(value / digit) % 10];

		UI_BillBoard* billBoard = new UI_BillBoard(graph, trs, center, clipSize);
		billBoard->SetEasing(UI_Canvas::eAlpha, 255, 0, 1.25f, EasingType::InCubic);

		UI_Manager::Detach(billBoard);

		numbers.push_back(billBoard);
		numbersPos.push_back(trs.position);

		digit *= 10;
	}
}
