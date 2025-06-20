#include "UI_HitPoint_Icon.h"
#include "src/common/camera/CameraManager.h"

UI_HitPoint_Icon::UI_HitPoint_Icon() : 
	UI_HitPoint_Icon(RectTransform(), -1)
{
}

UI_HitPoint_Icon::UI_HitPoint_Icon(const RectTransform& trs, int index)
{
	charaIndex = index;
	SetTransform(trs);
	SetValue(nullptr, 0, 0, 0);
}

UI_HitPoint_Icon::~UI_HitPoint_Icon()
{

}

void UI_HitPoint_Icon::Update()
{
	if (charaIndex >= 0) {

		if (CameraManager::IsScreenDivision()) {
			SetIsDraw(true);
			float scrWidth = (WindowSetting::Inst().width / CameraManager::AllCameras().size()) * charaIndex;
			*rectTransform = RectTransform(Anchor::Preset::LeftDown, Vector2(scrWidth, 0.0f), 0.0f, Vector2::Ones * 2.0f);
		}
		else if (charaIndex == 0) {
			SetIsDraw(true);
		}
		else {
			SetIsDraw(false);
		}
	}

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
		DrawFormatStringF(globalTrs.position.x + ImageSize().x + 5.0f, globalTrs.position.y - ImageSize().y * 0.5f, 0xFFFFFF, "x %d", *m_Value);
	}

	rectTransform->position = saveTrs.position;
}

void UI_HitPoint_Icon::SetValue(int* value, int valueMin, int valueMax, int dispNum)
{
	m_Value = value;
	m_ValueMin = valueMin;
	m_ValueMax = valueMax;
	m_DispNum = dispNum;
}
