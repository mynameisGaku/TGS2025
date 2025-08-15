#include "UI_ChatBar.h"
#include "SliceBar.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

UI_ChatBar::UI_ChatBar() : UI_ChatBar(RectTransform(), 0)
{
}

UI_ChatBar::UI_ChatBar(const RectTransform& trs, int index)
{
	m_hBarImage = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar.png");
	assert(m_hBarImage > 0);

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, index);

	m_BarBack = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2(100, 50), rectTransform));
	m_BarGaugeBack = new SliceBar(RectTransform(Vector2(0, -200), 0.0f, Vector2(300, 70), rectTransform));

	m_BarBack->InitImage(m_hBarImage);
	m_BarGaugeBack->InitImage(m_hBarImage);

	m_BarBack->SetCenter(Vector2(0, 0));
	m_BarGaugeBack->SetCenter(Vector2(1.0f, 0.5f));
}

UI_ChatBar::~UI_ChatBar()
{
	ResourceLoader::DeleteGraph(hImage);
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
