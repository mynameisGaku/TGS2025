#include "src/util/ui/UI_Canvas.h"
#include "src/util/ui/UI_Manager.h"
#include "src/common/setting/window/WindowSetting.h"

// ◇汎用
#include "framework/myDxLib.h"
#include <src/util/math/mathUtils.h>
#include <src/util/ptr/PtrUtil.h>

UI_Canvas::UI_Canvas() {

	//================================================================================
	// ◇画面サイズに応じて拡縮倍率を変化させる

	ScaleWithScreenSize();

	//================================================================================
	// ◇イージング挙動

	easingMove	= EasingVec2();
	easingScale = EasingVec2();
	easingRot	= EasingFloat();
	easingAlpha = EasingInt();

	//================================================================================
	// ◇点滅挙動

	blinkTime = 0.0f;
	blinkCounter = 0.0f;
	blinkSpeed = MathUtil::ToRadians(1.0f);

	needBlink = false;

	//================================================================================
	// ◇残像挙動

	easingMoveAftImg	= EasingVec2();
	easingScaleAftImg	= EasingVec2();
	easingRotAftImg		= EasingFloat();
	easingAlphaAftImg	= EasingInt();

	needAfterImage = false;
	
	//================================================================================
	// ◇変数's
	
	hImage = -1;
	priority = 0;
	alpha = 255;

	subtone = nullptr;

	isUpdate = true;
	isDraw = true;

	UI_Manager::Add(this);
}

UI_Canvas::~UI_Canvas() {

	UI_Manager::Detach(this);
	PtrUtil::SafeDelete(subtone);
}

void UI_Canvas::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	// 画像サイズに応じて拡縮倍率を変化させる
	ScaleWithScreenSize();

	// 点滅処理
	BlinkUpdate();

	// 補間処理
	EasingUpdate();

	// 残像描画処理
	AfterImageUpdate();

	Object2D::Update();
}

void UI_Canvas::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	if (subtone != nullptr)
		subtone->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	Object2D::Draw();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Canvas::BlinkUpdate() {

	//================================================================================
	// ◇点滅処理

	if (needBlink == false || blinkTimeMax == 0.0f)
		return;

	//================================================================================
	// ◇時間加算
	
	blinkTime += GTime.deltaTime;
	if (blinkTime >= blinkTimeMax)
		blinkTime = 0.0f;

	//================================================================================
	// ◇倍率計算

	float rate = blinkTime / (blinkTimeMax * 0.5f);	// 倍率

	if (rate >= 1.0f)
		rate = 1.0f - rate;

	alpha = static_cast<int>(75 * rate) + 180;
}

void UI_Canvas::EasingUpdate() {

	//================================================================================
	// ◇補間処理

	EasingFunc::Process(&easingMove.info,	&rectTransform->position,	easingMove.end,		easingMove.begin);
	EasingFunc::Process(&easingRot.info,	&rectTransform->rotation,	easingRot.end,		easingRot.begin);
	EasingFunc::Process(&easingScale.info,	&rectTransform->scale,		easingScale.end,	easingScale.begin);
	EasingFunc::Process(&easingAlpha.info,	&alpha,						easingAlpha.end,	easingAlpha.begin);
}

void UI_Canvas::AfterImageUpdate() {

	//================================================================================
	// ◇残像描画処理

	if (needAfterImage == false)
		return;

	easingMoveAftImg.Update();
	easingRotAftImg.Update();
	easingScaleAftImg.Update();
	easingAlphaAftImg.Update();

	if (IsAllEasingRunAfterImage() == false)
		needAfterImage = false;
}

void UI_Canvas::ScaleWithScreenSize() {

	// 画面情報を取得
	GetScreenState(&screenX, &screenY, &colorBitDepth);

	// 取得した画面情報と、既定の画面サイズで拡縮倍率を変化させる
	scaleWithScreenSize = Vector2(static_cast<float>(screenX) / static_cast<int>(WindowSetting::Inst().width), static_cast<float>(screenY) / static_cast<int>(WindowSetting::Inst().height));
}

void UI_Canvas::SubtoneAdaptation(const UI_Subtone& sub) {

	PtrUtil::SafeDelete(subtone);
	subtone = new UI_Subtone(sub);

	RectTransform globalTrs = rectTransform->Global();
	Vector2 devi = DisplacementByAnchorPoint();

	subtone->SetPoint(globalTrs.position + devi, globalTrs.position + devi + ImageSize());
}

void UI_Canvas::SetPriority(const int& value) {

	priority = value;
	UI_Manager::Sort();
}

void UI_Canvas::SetSubtone(const UI_Subtone& sub) {

	PtrUtil::SafeDelete(subtone);
	subtone = new UI_Subtone(sub);
}

bool UI_Canvas::IsAllEasingRun() const {

	bool isMove  = easingMove.info.isActive;
	bool isRot	 = easingRot.info.isActive;
	bool isScale = easingScale.info.isActive;
	bool isAlpha = easingAlpha.info.isActive;

	return (isMove || isRot || isScale || isAlpha);
}

bool UI_Canvas::IsAllEasingRunAfterImage() const {

	bool isMove  = easingMoveAftImg.info.isActive;
	bool isRot	 = easingRotAftImg.info.isActive;
	bool isScale = easingScaleAftImg.info.isActive;
	bool isAlpha = easingAlphaAftImg.info.isActive;

	return (isMove || isRot || isScale || isAlpha);
}
