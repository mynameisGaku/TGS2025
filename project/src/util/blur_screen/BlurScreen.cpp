#include "src/util/blur_screen/BlurScreen.h"
#include "src/util/time/GameTime.h"
#include "src/common/setting/window/WindowSetting.h"

//#include "settingManager.h"

namespace {

	static const int NUMBER_OF_DRAWINGS = 2;	// ブラー用スクリーンの描画枚数

	float alpha;		// 不透明度
	float defAlpha;		// 初期値不透明度
	int current;		// 現在使用しているスクリーンの要素数
	int notBlendDraw;	// ブラー用スクリーンを描画するか
	float duration;		// 効果時間
	float fadeDuration;
	float fadeTotalDuration;
	bool isFadeOut;		// フェード処理を行うか
	bool isUse;			// ブラーを使用しているか
	int screen[NUMBER_OF_DRAWINGS];	// ブラー用スクリーン
}

void BlurScreen::Init(float _alpha) {

	for (int i = 0; i < NUMBER_OF_DRAWINGS; i++) {
		screen[i] = MakeScreen(static_cast<int>(WindowSetting::Inst().width), static_cast<int>(WindowSetting::Inst().height), TRUE);
	}

	Reset();

	alpha = _alpha;
	defAlpha = _alpha;
}

void BlurScreen::ReplayInit(float _duration, float _fadeDuration) {

	Reset();

	for (int i = 0; i < NUMBER_OF_DRAWINGS; i++) {
		if (screen[i] != -1)
			DeleteGraph(screen[i]);

		screen[i] = MakeScreen(static_cast<int>(WindowSetting::Inst().width), static_cast<int>(WindowSetting::Inst().height), TRUE);
	}

	alpha = defAlpha;
	duration = _duration;
	fadeDuration = fadeTotalDuration = _fadeDuration;
	isUse = true;
}

void BlurScreen::Update() {

	if (isUse == false)
		return;

	PreRenderBlurScreen();

	//if (SettingManager::IsActive())
	//	return;

	duration = max(duration - GTime.deltaTime, 0.0f);
	if (!isFadeOut && duration <= 0.0f)
		FadeOut(fadeDuration);

	if (isFadeOut) {
		fadeDuration = max(fadeDuration - GTime.deltaTime, 0.0f);

		if (fadeTotalDuration != 0.0f)
			alpha = defAlpha * min(max((fadeDuration / fadeTotalDuration), 0.0f), 1.0f);

		if (fadeDuration <= 0.0f) {
			alpha = 0.0f;
			fadeDuration = 0.0f;
			isFadeOut = false;
			isUse = false;
			Release();
		}
	}
}

void BlurScreen::Draw() {

	PostRenderBlurScreen();
}

void BlurScreen::Release() {

	for (int i = 0; i < NUMBER_OF_DRAWINGS; i++) {
		if (screen[i] != -1) {
			DeleteGraph(screen[i]);
			screen[i] = -1;
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void BlurScreen::Reset() {

	Release();

	alpha = 0.0f;
	current = 0;
	notBlendDraw = 0;

	fadeDuration = 0.0f;
	fadeTotalDuration = 0.0f;

	isUse = false;
	isFadeOut = false;
}

void BlurScreen::PreRenderBlurScreen() {

	if (screen[current] != -1) {
		SetDrawScreen(screen[current]);
	}
}

void BlurScreen::PostRenderBlurScreen() {

	if (screen[current] == -1 || !isUse)
		return;

	int drawMode = GetDrawMode();

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	int blendMode, param;
	GetDrawBlendMode(&blendMode, &param);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha));

	if (notBlendDraw++ > 2)
		DrawExtendGraphF(0.0f, 0.0f, static_cast<int>(WindowSetting::Inst().width), static_cast<int>(WindowSetting::Inst().height), screen[1 - current], false);

	SetDrawBlendMode(blendMode, param);
	SetDrawMode(drawMode);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, screen[current], false);
	ScreenFlip();

	current = 1 - current;
}

void BlurScreen::FadeOut(float duration) {

	fadeDuration = fadeTotalDuration = duration;
	isFadeOut = true;
}

int BlurScreen::ScreenInfo(int i) {

	return screen[i];
}

float BlurScreen::Duration() {

	return duration;
}

bool BlurScreen::IsFadeOut() {

	return isFadeOut;
}

bool BlurScreen::IsUse() {

	return isUse;
}
