#include "fader.h"

#include "src/util/time/GameTime.h"
#include "src/util/easing/Easing.h"
#include "src/common/setting/window/WindowSetting.h"

namespace {

	//================================================================================
	// ▼フェード関連

	// フェードする色
	int color;

	// フェードの補間
	EasingFloat fadeEasing;

	//================================================================================
	// ▼レターボックス関連

	// レターボックスの数(上下)
	static const int LETTER_BOX_NUM = 2;

	// レターボックスの開始地点(左上)
	static const Vector2 LETTER_BOX_BIGIN_POS[LETTER_BOX_NUM] = { Vector2(0, 0), Vector2(0, static_cast<int>(WindowSetting::Inst().height) - 50) };

	// レターボックスの終了地点(右下)
	static const Vector2 LETTER_BOX_END_POS[LETTER_BOX_NUM] = { Vector2(static_cast<int>(WindowSetting::Inst().width), 50), Vector2(static_cast<int>(WindowSetting::Inst().width), static_cast<int>(WindowSetting::Inst().height)) };

	// レターボックス
	struct LetterBox {
		EasingVec2 leftUpCorner;
		EasingVec2 rightDownCorner;
	};

	LetterBox letterBoxes[LETTER_BOX_NUM];
}

void Fader::Init() {

	color = GetColor(0, 0, 0);

	fadeEasing.current = 255.0f;
	fadeEasing.begin = 255.0f;
	fadeEasing.end = 255.0f;
}

void Fader::Update() {

	fadeEasing.Update();

	for (int i = 0; i < LETTER_BOX_NUM; i++) {
		letterBoxes[i].leftUpCorner.Update();
		letterBoxes[i].rightDownCorner.Update();
	}
}

void Fader::Draw() {

	if (fadeEasing.current > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fadeEasing.current));
		DrawBox(0, 0, static_cast<int>(WindowSetting::Inst().width), static_cast<int>(WindowSetting::Inst().height), color, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	for (int i = 0; i < LETTER_BOX_NUM; i++) {
		DrawBoxAA(letterBoxes[i].leftUpCorner.current.x, letterBoxes[i].leftUpCorner.current.y,
			letterBoxes[i].rightDownCorner.current.x, letterBoxes[i].rightDownCorner.current.y, 0x000000, true);
	}
}

void Fader::Release() {

}

void Fader::FadeStart(float totalTime, EasingType easingType, float begin, float end, GameTime::AdditionMethod timeAddition) {

	fadeEasing.current = begin;

	fadeEasing.begin = begin;
	fadeEasing.end = end;
	fadeEasing.info.time = 0.0f;
	fadeEasing.info.totaltime = totalTime;
	fadeEasing.info.type = easingType;
	fadeEasing.info.additionMethod = timeAddition;
	fadeEasing.info.isActive = true;

	EasingFunc::Process(&fadeEasing.info, &fadeEasing.current, fadeEasing.end, fadeEasing.begin);
}

void Fader::LetterBoxStart(float totalTime, float size, EasingType easingType, GameTime::AdditionMethod timeAddition, bool fadeIn) {

	for (int i = 0; i < LETTER_BOX_NUM; i++) {

		int judge = (i % 2 == 0) ? 1 : -1;
		Vector2 adjust = Vector2(0.0f, size * -2.0f * judge);

		Vector2 leftUpCornerBegin;
		Vector2 leftUpCornerEnd;

		Vector2 rightDownCornerBegin;
		Vector2 rightDownCornerEnd;

		if (fadeIn) {

			leftUpCornerBegin = LETTER_BOX_BIGIN_POS[i] + adjust;
			leftUpCornerEnd = LETTER_BOX_BIGIN_POS[i];

			rightDownCornerBegin = LETTER_BOX_END_POS[i] + adjust;
			rightDownCornerEnd = LETTER_BOX_END_POS[i];
		}
		else {

			leftUpCornerBegin = LETTER_BOX_BIGIN_POS[i];
			leftUpCornerEnd = LETTER_BOX_BIGIN_POS[i] + adjust;

			rightDownCornerBegin = LETTER_BOX_END_POS[i];
			rightDownCornerEnd = LETTER_BOX_END_POS[i] + adjust;
		}

		letterBoxes[i].leftUpCorner.begin = leftUpCornerBegin;
		letterBoxes[i].leftUpCorner.end = leftUpCornerEnd;
		letterBoxes[i].leftUpCorner.info.time = 0.0f;
		letterBoxes[i].leftUpCorner.info.totaltime = totalTime;
		letterBoxes[i].leftUpCorner.info.type = easingType;
		letterBoxes[i].leftUpCorner.info.additionMethod = timeAddition;
		letterBoxes[i].leftUpCorner.info.isActive = true;

		letterBoxes[i].rightDownCorner.begin = rightDownCornerBegin;
		letterBoxes[i].rightDownCorner.end = rightDownCornerEnd;
		letterBoxes[i].rightDownCorner.info.time = 0.0f;
		letterBoxes[i].rightDownCorner.info.totaltime = totalTime;
		letterBoxes[i].rightDownCorner.info.type = easingType;
		letterBoxes[i].rightDownCorner.info.additionMethod = timeAddition;
		letterBoxes[i].rightDownCorner.info.isActive = true;
	}
}

bool Fader::IsPlaying() {

	return fadeEasing.info.isActive;
}

bool Fader::IsPlayingLetterBox() {

	for (int i = 0; i < LETTER_BOX_NUM; i++) {
		if (letterBoxes[i].leftUpCorner.info.isActive ||
			letterBoxes[i].rightDownCorner.info.isActive)
			return true;
	}

	return false;
}

bool Fader::IsDisplay() {

	return (fadeEasing.current > 0);
}
