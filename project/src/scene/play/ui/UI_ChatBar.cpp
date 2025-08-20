#include "UI_ChatBar.h"
#include "SliceBar.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/reference/ui/UI_ChatBarRef.h"
#include <src/util/input/InputManager.h>

UI_ChatBar::UI_ChatBar() : UI_ChatBar(RectTransform(), 0)
{
}

UI_ChatBar::UI_ChatBar(const RectTransform& trs, int index)
{
	m_hBarImage = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar.png");
	assert(m_hBarImage > 0);

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, index);

	m_BarList["Back"] = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
	m_BarList["GaugeBack"] = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));

	m_BarList["Back"]->SetCenter(Vector2(0.5f, 0.5f));
	m_BarList["GaugeBack"]->SetCenter(Vector2(0.5f, 0.5f));

	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		bar->InitImage(m_hBarImage);
	}

	ReloadParam();
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

void UI_ChatBar::ReloadParam()
{
	UI_CHATBAR_REF.Load(true);

	m_BarList["Back"]->rectTransform->scale = UI_CHATBAR_REF.BackScale;
	m_BarList["GaugeBack"]->rectTransform->scale = UI_CHATBAR_REF.GaugeBackScale;

	m_BarList["Back"]->Color = UI_CHATBAR_REF.BackColor;
	m_BarList["GaugeBack"]->Color = UI_CHATBAR_REF.GaugeBackColor;
}

void UI_ChatBar::Update()
{
#ifdef _DEBUG
	if (InputManager::Push(KeyDefine::KeyCode::R)) {
		ReloadParam();
	}
#endif

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
