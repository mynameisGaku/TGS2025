#include "UI_HitPoint_Icon.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/screen/ScreenManager.h"

UI_HitPoint_Icon::UI_HitPoint_Icon() : 
	UI_HitPoint_Icon(RectTransform(), -1)
{
}

UI_HitPoint_Icon::UI_HitPoint_Icon(const RectTransform& trs, int index)
{
	charaIndex = index;

	Vector2 beginPos = ScreenManager::GetScreenBeginPos(charaIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(charaIndex);
	Vector2 size = endPos - beginPos;

	RectTransform rectTrs = trs;
	rectTrs.anchor.SetBegin(beginPos);
	rectTrs.anchor.SetEnd(endPos);

	SetTransform(rectTrs);
	SetValue(nullptr, 0.0f, 0.0f, 0.0f);
}

UI_HitPoint_Icon::~UI_HitPoint_Icon()
{

}

void UI_HitPoint_Icon::Update()
{
	UI_Canvas::Update();
}

void UI_HitPoint_Icon::Draw()
{
	if (m_Value == nullptr)
		return;

	RectTransform saveTrs = *rectTransform;
	const RectTransform globalTrs = rectTransform->Global();
	Vector2 offset = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	if (*m_Value <= m_ValueMax)
	{
		RectTransform dispTrs = globalTrs;

		for (int i = 0; i < *m_Value; i++) {
			rectTransform->position.x = saveTrs.position.x + ImageSize().x * i;
			UI_Canvas::Draw();
		}
	}
	else
	{
		UI_Canvas::Draw();
		DrawFormatStringF(globalTrs.position.x + ImageSize().x + 5.0f, globalTrs.position.y - ImageSize().y * 0.5f, 0xFFFFFF, "x %0f", *m_Value);
	}

	rectTransform->position = saveTrs.position;
}

void UI_HitPoint_Icon::SetValue(float* value, float valueMin, float valueMax, float dispNum)
{
	m_Value = value;
	m_ValueMin = valueMin;
	m_ValueMax = valueMax;
	m_DispNum = dispNum;
}
