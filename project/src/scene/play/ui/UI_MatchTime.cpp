#include "UI_MatchTime.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"

UI_MatchTime::UI_MatchTime() : UI_MatchTime(RectTransform(), 0)
{
}

UI_MatchTime::UI_MatchTime(const RectTransform& trs, int index)
{
	m_pMatchManager = nullptr;
	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);
}

UI_MatchTime::~UI_MatchTime()
{
}

void UI_MatchTime::Update()
{
	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_MatchTime::Draw()
{
	UI_Canvas::Draw();

	if (m_pMatchManager == nullptr) {
		m_pMatchManager = FindGameObject<MatchManager>();

		if (m_pMatchManager == nullptr)
			return;
	}

	std::string text = StringUtil::FormatToString("残り時間:%0.2f", m_pMatchManager->GetPlayTimeMaxSec() - m_pMatchManager->GetPlayTimeSec()).c_str();
	int width = GetDrawStringWidth(text.c_str(), text.length());

	const Vector2 adjust = Vector2(0, 0);
	const RectTransform globalTrs = rectTransform->Global();

	DrawBoxAA(
		globalTrs.position.x - width,
		globalTrs.position.y - adjust.y * 0.5f,
		globalTrs.position.x,
		globalTrs.position.y + adjust.y * 0.5f,
		GetColor(120, 120, 120), true);

	DrawFormatString(
		globalTrs.position.x - width,
		globalTrs.position.y - adjust.y * 0.5f,
		GetColor(255, 255, 255),
		text.c_str());
}