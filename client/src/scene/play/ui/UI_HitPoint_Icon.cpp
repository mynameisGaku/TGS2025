#include "UI_HitPoint_Icon.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/ui/UI_Manager.h"

UI_HitPoint_Icon::UI_HitPoint_Icon() : 
	UI_HitPoint_Icon(RectTransform(), -1)
{
}

UI_HitPoint_Icon::UI_HitPoint_Icon(const RectTransform& trs, int index)
{
	m_CharaIndex = index;
	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	SetValue(nullptr, 0.0f, 0.0f, 0.0f);

	m_NeedRelocation = false;
}

UI_HitPoint_Icon::~UI_HitPoint_Icon()
{

}

void UI_HitPoint_Icon::Update()
{
	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_HitPoint_Icon::Draw()
{
	if (m_pValue == nullptr)
		return;

	RectTransform saveTrs = *rectTransform;
	const RectTransform globalTrs = rectTransform->Global();
	Vector2 offset = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	if (*m_pValue <= m_ValueMax)
	{
		RectTransform dispTrs = globalTrs;

		for (int i = 0; i < *m_pValue; i++) {
			rectTransform->position.x = saveTrs.position.x + ImageSize().x * i;
			UI_Canvas::Draw();
		}
	}
	else
	{
		UI_Canvas::Draw();
		DrawFormatStringF(globalTrs.position.x + ImageSize().x + 5.0f, globalTrs.position.y - ImageSize().y * 0.5f, 0xFFFFFF, "x %0f", *m_pValue);
	}

	rectTransform->position = saveTrs.position;
}

void UI_HitPoint_Icon::SetValue(float* value, float valueMin, float valueMax, float dispNum)
{
	m_pValue = value;
	m_ValueMin = valueMin;
	m_ValueMax = valueMax;
	m_DispNum = dispNum;
}
