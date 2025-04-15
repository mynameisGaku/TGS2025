#include "SettingBase.h"

SettingBase::SettingBase() {

	fontInfo.strData.tag = "�����Ȃݖ���";
	fontInfo.size = 36;
	fontInfo.fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;

	Font::CreateFontToHandle(&fontInfo);

	isAdjustable = false;
}

SettingBase::~SettingBase() {

	for (auto& itr : sliders) {
		// UI_Manager����؂藣��
		UI_Manager::Detach(itr);

		// ���̂��폜����
		Function::DeletePointer(itr);
	}

	sliders.clear();
}

void SettingBase::Update() {

	// �C�[�W���O�����̍X�V
	alphaEasing.Update();

	for (auto& itr : sliders) {
		// �������Ă��Ȃ��ꍇ
		if (itr->IsActive() == false)
			continue;

		// �s�����x��K��
		itr->SetAlpha(alphaEasing.current);

		// �s�����x��0�ȉ��̏ꍇ
		if (alphaEasing.current <= 0) {
			itr->SetAlpha(0);
			itr->SetIsActive(false);
		}
	}
}

void SettingBase::FadeIn() {

	isAdjustable = true;
	alphaEasing.SetEasing(0, 255, 0.25f, EasingType::Linear, true);

	for (auto& itr : sliders) {
		itr->SetIsActive(true);
		itr->SetIsUpdate(true);
	}
}

void SettingBase::FadeOut() {

	isAdjustable = false;
	alphaEasing.SetEasing(255, 0, 0.25f, EasingType::Linear, true);

	for (auto& itr : sliders)
		itr->SetIsUpdate(false);
}
