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

	m_BarList["Back"] = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2(100, 50), rectTransform));
	m_BarList["GaugeBack"] = new SliceBar(RectTransform(Vector2(0, -200), 0.0f, Vector2(300, 70), rectTransform));

	m_BarList["Back"]->SetCenter(Vector2(0, 0));
	m_BarList["GaugeBack"]->SetCenter(Vector2(1.0f, 0.5f));

	m_BarList["Back"]->Color = RGBColor(50);
	m_BarList["GaugeBack"]->Color = RGBColor(100);

	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		bar->InitImage(m_hBarImage);
	}
}

UI_ChatBar::~UI_ChatBar()
{
	ResourceLoader::DeleteGraph(hImage);
	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		delete bar;
	}
}

void UI_ChatBar::Update()
{
	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		bar->Update();
	}
}

void UI_ChatBar::Draw()
{
	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		bar->Draw();
	}
}
