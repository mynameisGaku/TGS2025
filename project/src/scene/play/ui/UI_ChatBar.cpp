#include "UI_ChatBar.h"
#include "SliceBar.h"
#include "src/util/ui/UI_Manager.h"

UI_ChatBar::UI_ChatBar() : UI_ChatBar(RectTransform(), 0)
{
}

UI_ChatBar::UI_ChatBar(const RectTransform& trs, int index)
{
	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, index);

	m_BarBack = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
	m_BarGaugeBack = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
}

UI_ChatBar::~UI_ChatBar()
{
	delete m_BarBack;
	delete m_BarGaugeBack;
}

void UI_ChatBar::Update()
{
	m_BarBack->Update();
	m_BarGaugeBack->Update();
}

void UI_ChatBar::Draw()
{
	m_BarBack->Draw();
	m_BarGaugeBack->Draw();
}
