#include "UI_Fade.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/screen/ScreenManager.h"

namespace
{
	static const float FADE_DURATION = 1.0f;
}

UI_FadeBase::UI_FadeBase() : UI_FadeBase(RectTransform(), 0)
{
}

UI_FadeBase::UI_FadeBase(const RectTransform& trs, int index)
{
	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	m_FadeState = st_NONE;
	m_FadeTime = 0.0f;
}

UI_FadeBase::~UI_FadeBase()
{
}

void UI_FadeBase::Update()
{
	// 時間進める
	m_FadeTime += GTime.DeltaTime();
	if (m_FadeTime > FADE_DURATION)
	{
		m_FadeTime = FADE_DURATION;
	}

	// 0..1に換算
	float rate = 0.0f;
	if (FADE_DURATION <= 0)
	{
		rate = 1.0f;
	}
	else
	{
		rate = m_FadeTime / FADE_DURATION;
	}

	// フェードステートに応じて処理
	switch (m_FadeState)
	{
	case UI_FadeBase::st_NONE:
		break;
	case UI_FadeBase::st_FADEOUT:
		fadeOut(rate);
		break;
	case UI_FadeBase::st_FADEIN:
		fadeIn(rate);
		break;
	default:
		break;
	}
}

void UI_FadeBase::StartFadeOut()
{
	m_FadeState = st_FADEOUT;
	m_FadeTime = 0.0f;
}

void UI_FadeBase::StartFadeIn()
{
	m_FadeState = st_FADEIN;
	m_FadeTime = 0.0f;
}

bool UI_FadeBase::IsFadeEnd() const
{
    return m_FadeTime >= FADE_DURATION;
}

UI_FadeBlack::UI_FadeBlack() : UI_FadeBlack(RectTransform(), 0)
{
}

UI_FadeBlack::UI_FadeBlack(const RectTransform& trs, int index) : UI_FadeBase(trs, index)
{
	m_Alpha = 0;
}

UI_FadeBlack::~UI_FadeBlack()
{
}

void UI_FadeBlack::Draw()
{
	const Vector2 origin = ScreenManager::GetScreenBeginPos(m_CharaIndex);
	const Vector2 dest = ScreenManager::GetScreenEndPos(m_CharaIndex);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_Alpha);
	DrawBoxAA(origin.x, origin.y, dest.x, dest.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_FadeBlack::fadeIn(float rate)
{
	m_Alpha = (int)((1.0f - rate) * 255);
}

void UI_FadeBlack::fadeOut(float rate)
{
	m_Alpha = (int)(rate * 255);
}
