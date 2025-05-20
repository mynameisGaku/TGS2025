#include "SettingBase.h"

SettingBase::SettingBase() {

	fontInfo.strData.tag = "さざなみ明朝";
	fontInfo.size = 36;
	fontInfo.fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;

	Font::CreateFontToHandle(&fontInfo);

	isAdjustable = false;
}

SettingBase::~SettingBase() {

	for (auto& itr : sliders) {
		// UI_Managerから切り離す
		UI_Manager::Detach(itr);

		// 実体を削除する
		Function::DeletePointer(itr);
	}

	sliders.clear();
}

void SettingBase::Update() {

	// イージング挙動の更新
	alphaEasing.Update();

	for (auto& itr : sliders) {
		// 活動していない場合
		if (itr->IsActive() == false)
			continue;

		// 不透明度を適応
		itr->SetAlpha(alphaEasing.current);

		// 不透明度が0以下の場合
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
