#include "UI_StartCount.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/easing/easing.h"
#include <src/util/font/Font.h>

namespace
{
	static const float START_DEFAULT_SCALE = 0.5f;

	static const float START_MOVING_SCALE = 1.0f;
	static const float START_MOVING_TIME = 1.0f;

	static const float SCALE_DEFAULT = 1.0f;

	static const float FADE_TIME = 0.1f;
}

UI_StartCount::UI_StartCount() : UI_StartCount(RectTransform(), 0)
{
}

UI_StartCount::UI_StartCount(const RectTransform& trs, int index)
{
	m_pMatchManager = nullptr;
	m_CharaIndex = index;
	m_Count = 0;
	m_Scale = SCALE_DEFAULT;
	m_Time = 0.0f;

	FontInfo fontInfo = Font::BasicFont();
	fontInfo.SetCharSet(DEFAULT_CHARSET).SetSize(192).SetFontType(DX_FONTTYPE_ANTIALIASING_EDGE_16X16);
	m_hFont = Font::Create(fontInfo, "StartCount");

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

	if (m_pMatchManager == nullptr) {
		m_pMatchManager = FindGameObject<MatchManager>();

		if (m_pMatchManager == nullptr)
			return;
	}

	float timer = m_pMatchManager->GetReadyTimerSec();
	m_Count = (int)ceilf(timer);

	if (m_Count > 0)
	{
		m_Time = 0;
		m_Scale = SCALE_DEFAULT;
	}
	else
	{
		m_Time += GTime.DeltaTime();

		m_Scale = EasingFunc::OutQuint(m_Time, START_MOVING_TIME, START_MOVING_SCALE, START_DEFAULT_SCALE);
	}

	UI_Canvas::Update();
}

void UI_StartCount::Draw()
{
	UI_Canvas::Draw();

	std::string text = "";
	int alpha = 0;

	if (m_Count > 0)
	{
		text = StringUtil::FormatToString("%d", m_Count).c_str();
		alpha = 255;
	}
	else if (m_Time < START_MOVING_TIME)
	{
		text = "START!";

		if (m_Time < FADE_TIME)
		{
			alpha = (int)(m_Time / FADE_TIME * 255);
		}
		else if (START_MOVING_TIME - m_Time < FADE_TIME)
		{
			alpha = (int)((START_MOVING_TIME - m_Time) / FADE_TIME * 255);
		}
		else
		{
			alpha = 255;
		}
	}

	int width = 0;
	int height = 0;
	GetDrawStringSizeToHandle(&width, &height, nullptr, text.c_str(), (int)text.length(), m_hFont);

	const RectTransform globalTrs = rectTransform->Global();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendFormatStringToHandle(
		(int)(globalTrs.position.x - width * m_Scale * 0.5f),	// 中央揃え
		(int)(globalTrs.position.y - height * m_Scale * 0.5f),
		m_Scale,
		m_Scale,
		GetColor(255, 255, 255),
		m_hFont,
		text.c_str());

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool UI_StartCount::isStart() const
{
	return m_Count <= 0;
}
