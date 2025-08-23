#include "UI_MatchTime.h"
#include "SliceBar.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/reference/ui/UI_MatchTimeRef.h"
#include <src/util/input/InputManager.h>

UI_MatchTime::UI_MatchTime() : UI_MatchTime(RectTransform(), 0)
{
}

UI_MatchTime::UI_MatchTime(const RectTransform& trs, int index)
{
	m_hBarImage = ResourceLoader::LoadGraph("data/texture/UI/ChatBar/bar.png");
	assert(m_hBarImage > 0);

	m_pMatchManager = nullptr;
	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	m_BackBar = new SliceBar(RectTransform(Vector2::Zero, 0.0f, Vector2::Ones, rectTransform));
	m_BackBar->SetCenter(Vector2(0.5f, 0.5f));
	m_BackBar->InitImage(m_hBarImage);
	m_BackBar->SetPriority(Priority() - 1);

	ReloadParam();
}

UI_MatchTime::~UI_MatchTime()
{
	ResourceLoader::DeleteGraph(hImage);
	delete m_BackBar;
}

void UI_MatchTime::Update()
{
#ifdef _DEBUG
	if (InputManager::Push(KeyDefine::KeyCode::R)) {
		ReloadParam();
	}
#endif

	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_MatchTime::Draw()
{
	UI_Canvas::Draw();

	//m_BackBar->Draw();

	if (m_pMatchManager == nullptr) {
		m_pMatchManager = FindGameObject<MatchManager>();

		if (m_pMatchManager == nullptr)
			return;
	}

	int matchTime = (int)ceilf(m_pMatchManager->GetPlayTimeMaxSec() - m_pMatchManager->GetPlayTimeSec());
	std::string text = StringUtil::FormatToString("%d", matchTime).c_str();
	int width = GetDrawStringWidth(text.c_str(), (int)text.length());

	const Vector2 adjust = Vector2(0, 0);
	const RectTransform globalTrs = rectTransform->Global();

	/*
	DrawBoxAA(
		globalTrs.position.x - width,
		globalTrs.position.y - adjust.y * 0.5f,
		globalTrs.position.x,
		globalTrs.position.y + adjust.y * 0.5f,
		GetColor(120, 120, 120), true);
		*/

	uint32_t color = (uint32_t)UI_MATCHTIME_REF.TimeNormalColor.GetColorInt();
	if (matchTime < 10)
		color = (uint32_t)UI_MATCHTIME_REF.TimeWarningColor.GetColorInt();
	else if (matchTime < 60)
		color = 0xffff00;

	DrawFormatString(
		(int)(globalTrs.position.x - width * 0.5f),
		(int)(globalTrs.position.y - adjust.y * 0.5f),
		color,
		text.c_str());
}

void UI_MatchTime::ReloadParam()
{
	UI_MATCHTIME_REF.Load(true);

	m_BackBar->Color = UI_MATCHTIME_REF.BackColor;
	m_BackBar->rectTransform->scale = UI_MATCHTIME_REF.BackScale;
}
