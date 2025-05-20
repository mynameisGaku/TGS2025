#include "src/util/ui/UI_Slider.h"
#include "src/util/ui/UI_Manager.h"

// ◇汎用
#include "framework/myDxLib.h"
#include "src/util/Utils.h"

// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/input/MouseController.h"

using namespace KeyDefine;

namespace {

	static const float ADJUST = 4.0f;	// 調整値
}

UI_Slider::UI_Slider(const std::string& _label, const RectTransform& trs, float* _value, const float& _minValue, const float& _maxValue, const Vector2& _backSize, const UI_Define::SliderColor& color, const std::string& _format, const FontInfo& _fontInfo) {

	Object2D::SetTransform(trs);

	label = _label;
	format = _format;

	value = _value;
	minValue = _minValue;
	maxValue = _maxValue;
	divisions = 5;

	colors = color;
	fontInfo = _fontInfo;

	SetPointEnabled(true);
	SetPointSize(Vector2(_backSize.x / 25.0f, _backSize.y + ADJUST));
	SetBackSize(_backSize);

	// フォントデータを生成する
	Font::CreateFontToHandle(&fontInfo);

	SetFormatStr(format, fontInfo);
	SetLabelText(label, fontInfo);

	UI_Manager::Sort();
}

UI_Slider::~UI_Slider() {

	Function::DeletePointer(labelText);
	Function::DeletePointer(formatText);
}

void UI_Slider::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	switch (touchPhase) {
	case TouchPhase::Begin:		TouchBeginUpdate();			break;
	case TouchPhase::Moved:		TouchMovedUpdate();			break;
	case TouchPhase::Ended:		TouchEndedUpdate();			break;
	default:
		break;
	}

	if (formatText != nullptr) {
		formatText->SetText(Function::FormatToString(format.c_str(), *value));
		formatText->Update();
	}

	if (labelText != nullptr) {
		labelText->Update();
	}

	UI_Canvas::Update();
}

void UI_Slider::TouchBeginUpdate() {

	pointPosition = Vector2((rectTransform->Global().position.x - backSize.x) + (*value - minValue) / (maxValue - minValue) * (backSize.x * 2), rectTransform->Global().position.y);

	if (MouseController::ColCheck_CursorToCircle(pointPosition, pointSize.Average()) && InputManager::Push(KeyCode::LeftClick))
		touchPhase = TouchPhase::Moved;
}

void UI_Slider::TouchMovedUpdate() {

	if (InputManager::Hold(KeyCode::LeftClick)) {
		pointPosition.x = MouseController::Info().position.x;

		if (pointPosition.x > rectTransform->Global().position.x + backSize.x)
			pointPosition.x = rectTransform->Global().position.x + backSize.x;
		else if (pointPosition.x < rectTransform->Global().position.x - backSize.x)
			pointPosition.x = rectTransform->Global().position.x - backSize.x;

		*value = (pointPosition.x - (rectTransform->Global().position.x - backSize.x)) / (backSize.x * 2.0f) * (maxValue - minValue) + minValue;

		if (*value < minValue)
			*value = minValue;
		else if (*value > maxValue)
			*value = maxValue;
	}
	else
		touchPhase = TouchPhase::Ended;
}

void UI_Slider::TouchEndedUpdate() {

	pointPosition = Vector2((rectTransform->Global().position.x - backSize.x) + (*value - minValue) / (maxValue - minValue) * (backSize.x * 2), rectTransform->Global().position.y);

	touchPhase = TouchPhase::Begin;
}

void UI_Slider::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawBack();
	DrawDivisions();
	DrawPoint();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (formatText != nullptr) {
		formatText->SetAlpha(alpha);
		formatText->Draw();
	}

	if (labelText != nullptr) {
		labelText->SetAlpha(alpha);
		labelText->Draw();
	}
}

void UI_Slider::DrawBack() {

	RectTransform globalTrs = rectTransform->Global();

	Vector2 backBeginPos = globalTrs.position - backSize;
	Vector2 backEndPos	 = globalTrs.position + backSize;

	DrawBoxAA(backBeginPos.x - 2.0f, backBeginPos.y - 4.0f, backEndPos.x + 2.0f, backEndPos.y + 4.0f, GetColor(0, 0, 0), true);
	DrawBoxAA(backBeginPos.x, backBeginPos.y, backEndPos.x, backEndPos.y, colors.backColor, true);

	Vector2 fillBeginPos = globalTrs.position - backSize + Vector2::Ones;
	Vector2 fillEndPos	 = Vector2::Zero;
	fillEndPos.x = pointPosition.x - 1;
	fillEndPos.y = globalTrs.position.y + backSize.y - 1;

	DrawBoxAA(fillBeginPos.x, fillBeginPos.y, fillEndPos.x, fillEndPos.y, colors.gaugeColor, true);

}

void UI_Slider::DrawDivisions() {

	const float DivSpace = (backSize.x * 2.0f) / (max(divisions - 1, 1));

	Vector2 glidLine = Vector2::Zero;
	Vector2 drawPos = rectTransform->Global().position - Vector2(backSize.x, 0.0f);

	for (int i = 0; i < divisions; i++) {

		Vector2 begin = Vector2(drawPos.x + glidLine.x + 2, drawPos.y - backSize.y - 1);
		Vector2 end = Vector2(drawPos.x + glidLine.x - 2, drawPos.y + backSize.y + 1);

		DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(0, 0, 0), true);

		glidLine.x += DivSpace;
	}
}

void UI_Slider::DrawPoint() {

	if (pointEnabled == false)
		return;

	Vector2 begin = pointPosition - pointSize;
	Vector2 end = pointPosition + pointSize;

	DrawBoxAA(begin.x, begin.y, end.x, end.y, colors.pointColor, true);
}

void UI_Slider::SetFormatStr(const std::string& _format, const FontInfo& font) {

	format = _format;

	int labelWidth = GetDrawFormatStringWidthToHandle(font.handle, format.c_str());

	RectTransform formatTrs;
	formatTrs.position.x = rectTransform->Global().position.x - ADJUST - (labelWidth * 0.5f);
	formatTrs.position.y = rectTransform->Global().position.y - ADJUST;

	Function::DeletePointer(formatText);

	formatText = new UI_Text(Function::FormatToString(format.c_str(), *value), formatTrs, font);
	formatText->SetParent(this);
	formatText->SetPriority(100);

	UI_Manager::Detach(formatText);
}

void UI_Slider::SetLabelText(const std::string& _label, const FontInfo& font) {

	label = _label;

	int labelWidth = GetDrawFormatStringWidthToHandle(font.handle, label.c_str());

	RectTransform labelTrs;
	labelTrs.position.x = rectTransform->Global().position.x - (backSize.x + labelWidth + ADJUST + 40.0f);
	labelTrs.position.y = rectTransform->Global().position.y;

	Function::DeletePointer(labelText);
	
	labelText = new UI_Text(Function::FormatToString(label.c_str(), *value), labelTrs, font);
	labelText->SetParent(this);
	labelText->SetPriority(100);

	UI_Manager::Detach(labelText);
}
