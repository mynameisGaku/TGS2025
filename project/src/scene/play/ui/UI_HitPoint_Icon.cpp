#include "UI_HitPoint_Icon.h"

UI_HitPoint_Icon::UI_HitPoint_Icon() : 
	UI_HitPoint_Icon(RectTransform())
{
}

UI_HitPoint_Icon::UI_HitPoint_Icon(const RectTransform& trs)
{
	SetValue(nullptr, 0, 0, 0);
}

UI_HitPoint_Icon::~UI_HitPoint_Icon()
{

}

void UI_HitPoint_Icon::Update()
{
}

void UI_HitPoint_Icon::Draw()
{
	if (m_Value == nullptr)
		return;

	const RectTransform globalTrs = rectTransform->Global();
	Vector2 offset = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	Vector2 dispPos = globalTrs.position + offset;

	if (*m_Value < m_ValueMax)
	{
		for (int i = 0; i < *m_Value; i++) {
			DrawRectRotaGraphF(dispPos.x + imageSize.x * i, dispPos.y + imageSize.y * i,
				0, 0, (int)imageSize.x, (int)imageSize.y, globalTrs.scale.Average(), globalTrs.rotation, hImage, true);
		}
	}
	else
	{
		DrawRectRotaGraphF(dispPos.x, dispPos.y, 0, 0, (int)imageSize.x, (int)imageSize.y, globalTrs.scale.Average(), globalTrs.rotation, hImage, true);
		DrawFormatStringF(dispPos.x + 50.0f, dispPos.y, 0xFFFFFF, "x %d", *m_Value);
	}
}

void UI_HitPoint_Icon::SetValue(int* value, int valueMin, int valueMax, int dispNum)
{
	m_Value = value;
	m_ValueMin = valueMin;
	m_ValueMax = valueMax;
	m_DispNum = dispNum;
}
