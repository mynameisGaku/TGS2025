#include "UI_StartCount.h"
#include "src/reference/game/GameRef.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/easing/easing.h"

namespace
{
	static const float START_DEFAULT_SCALE = 3.0f;

	static const float START_MOVING_SCALE = 5.0f;
	static const float START_MOVING_TIME = 1.0f;

	static const float SCALE_DEFAULT = 5.0f;

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
	m_Scale = 5.0f;
	m_Time = 0.0f;

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
		m_Scale = 5.0f;
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

	std::string text;
	int alpha = 0;

	if (m_Count > 0)
	{
		text = StringUtil::FormatToString("%d", m_Count).c_str();
		alpha = 255;
	}
	else if (m_Time < START_MOVING_TIME)
	{
		text = "行け！";

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

	int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * m_Scale);

	const Vector2 adjust = Vector2(0, 0);
	const RectTransform globalTrs = rectTransform->Global();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendFormatString(
		globalTrs.position.x - width * 0.5f,	// 中央揃え
		globalTrs.position.y - adjust.y * 0.5f,
		m_Scale,
		m_Scale,
		GetColor(255, 255, 255),
		text.c_str());

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool UI_StartCount::isStart() const
{
	return m_Count <= 0;
}
