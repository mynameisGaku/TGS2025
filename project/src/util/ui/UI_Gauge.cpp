#include "src/util/ui/UI_Gauge.h"

// ◇汎用
#include "framework/myDxLib.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/util/Utils.h"

// ◇演出・機能
#include "src/util/input/InputManager.h"

UI_Gauge::UI_Gauge(const RectTransform& trs, float* _value, const float& _minValue, const float& _maxValue, const Vector2& _gaugeSize, const UI_Define::SliderColor& color, const float& _gaugeThickness, const float& _gaugeLateSpeed, const int& _innerEdgeColor, const int& _outerEdgeColor) {

	Object2D::SetTransform(trs);

	gauge = Gauge();

	value	 = _value;
	minValue = _minValue;
	maxValue = _maxValue;

	hGauge			= -1;
	hDelayColor		= -1;
	hIncreaseColor	= -1;
	hBackColor		= -1;

	gaugeSize		= _gaugeSize;

	colors = color;

	innerEdgeColor	= _innerEdgeColor;
	outerEdgeColor	= _outerEdgeColor;

	gaugeThickness = _gaugeThickness;
	gaugeLateSpeed = _gaugeLateSpeed;
}

UI_Gauge::UI_Gauge(const RectTransform& trs, float* _value, const float& _minValue, const float& _maxValue, const int& gaugeGraph, const int& delayGraph, const int& increaseGraph, const int& backGraph, const float& _gaugeLateSpeed) {

	Object2D::SetTransform(trs);

	value	 = _value;
	minValue = _minValue;
	maxValue = _maxValue;

	hGauge			= gaugeGraph;
	hDelayColor		= backGraph;
	hIncreaseColor	= backGraph;
	hBackColor		= backGraph;

	gaugeSize		= Vector2::Zero;
	colors = UI_Define::SliderColor();

	innerEdgeColor	= GetColor(0, 0, 0);
	outerEdgeColor	= GetColor(255, 255, 255);

	gaugeThickness = 1.0f;
	gaugeLateSpeed = _gaugeLateSpeed;
}

UI_Gauge::~UI_Gauge() {

	ResourceLoader::DeleteGraph(hGauge);
	ResourceLoader::DeleteGraph(hDelayColor);
	ResourceLoader::DeleteGraph(hIncreaseColor);
	ResourceLoader::DeleteGraph(hBackColor);
}

void UI_Gauge::Update() {

	if (IsActive() == false)
		return;

	UI_Canvas::Update();
}

void UI_Gauge::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	RectTransform globalTrs = rectTransform->Global();
	Vector2 dispPos = globalTrs.position;

	switch (Anchor::PlacementHorizontal(rectTransform->preset)) {
	case Anchor::Placement::RIGHT:	dispPos.x -= gaugeSize.x;	break;
	case Anchor::Placement::MIDDLE:	dispPos.x -= gaugeSize.x * 0.5f;	break;
	case Anchor::Placement::LEFT:
	default:
		break;
	}

	switch (Anchor::PlacementVertical(rectTransform->preset)) {
	case Anchor::Placement::UP:		dispPos.y += gaugeSize.y;		break;
	case Anchor::Placement::MIDDLE:	dispPos.y -= gaugeSize.y * 0.5f;break;
	case Anchor::Placement::DOWN:	dispPos.y -= gaugeSize.y * 0.5f;break;
	default:
		break;
	}

	if (hGauge == -1 || hDelayColor == -1 || hIncreaseColor == -1 || hBackColor == -1)
		gauge.DrawGauge(dispPos, gaugeSize, *value, maxValue, minValue, colors.gaugeColor, colors.delayColor, colors.increaseColor, colors.backColor, gaugeLateSpeed, gaugeThickness, innerEdgeColor, outerEdgeColor);
	else
		gauge.DrawGraphGauge(dispPos, *value, maxValue, minValue, hGauge, hDelayColor, hIncreaseColor, hBackColor, gaugeLateSpeed);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
