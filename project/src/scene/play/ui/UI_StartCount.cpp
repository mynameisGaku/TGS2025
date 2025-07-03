#include "UI_StartCount.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"

UI_StartCount::UI_StartCount() : UI_StartCount(RectTransform(), 0)
{
}

UI_StartCount::UI_StartCount(const RectTransform& trs, int index)
{
	m_pMatchManager = nullptr;
	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);
}

UI_StartCount::~UI_StartCount()
{
}

void UI_StartCount::Update()
{
	// 画面分割数切り替え時にアンカーの位置を更新(デバッグ用)
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();
}

void UI_StartCount::Draw()
{
	UI_Canvas::Draw();

	if (m_pMatchManager == nullptr) {
		m_pMatchManager = FindGameObject<MatchManager>();

		if (m_pMatchManager == nullptr)
			return;
	}

	int count = 0;

	std::string text = StringUtil::FormatToString("%d", count).c_str();
	int width = GetDrawStringWidth(text.c_str(), text.length());

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

	DrawFormatString(
		globalTrs.position.x - width * 0.5f,	// 中央揃え
		globalTrs.position.y - adjust.y * 0.5f,
		GetColor(255, 255, 255),
		text.c_str());
}
