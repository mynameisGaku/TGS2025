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
	m_hBarImageBack = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar_grad_edge.png");
	assert(m_hBarImageBack > 0);
	m_hBarImageGaugeBack = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar.png");
	assert(m_hBarImageGaugeBack > 0);
	m_hIconImageThrow = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/throw_linear.png");
	assert(m_hIconImageThrow > 0);
	m_hIconImageCatch = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/catch.png");
	assert(m_hIconImageCatch > 0);

	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	m_BarList["Back"] = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
	m_BarList["GaugeBack"] = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));

	m_BarList["Back"]->SetCenter(Vector2(0.5f, 0.5f));
	m_BarList["GaugeBack"]->SetCenter(Vector2(0.5f, 0.5f));

	m_BarList["Back"]->InitImage(m_hBarImageBack);
	m_BarList["GaugeBack"]->InitImage(m_hBarImageGaugeBack);
	
	for (auto& item : m_BarList)
	{
		SliceBar* bar = item.second;
		bar->SetIsDraw(false);
	}

	ReloadParam();
}

UI_ChatBar::~UI_ChatBar()
{
	ResourceLoader::DeleteGraph(m_hBarImageBack);
	ResourceLoader::DeleteGraph(m_hBarImageGaugeBack);
	ResourceLoader::DeleteGraph(m_hIconImageThrow);
	ResourceLoader::DeleteGraph(m_hIconImageCatch);
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

	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

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

	RectTransform global = rectTransform->Global();

	DrawRotaGraphF(global.position.x - UI_CHATBAR_REF.IconOffsetX, global.position.y, UI_CHATBAR_REF.IconScale, 0.0f, m_hIconImageCatch, TRUE);
	DrawRotaGraphF(global.position.x + UI_CHATBAR_REF.IconOffsetX, global.position.y, UI_CHATBAR_REF.IconScale, 0.0f, m_hIconImageThrow, TRUE);
}
