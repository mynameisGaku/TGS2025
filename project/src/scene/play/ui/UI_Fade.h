#pragma once
#include "src/util/ui/UI_Canvas.h"

/// <summary>
/// 画面フェード
/// </summary>
/// <author>佐藤紘斗</author>
class UI_FadeBase : public UI_Canvas 
{
public:
	UI_FadeBase();
	UI_FadeBase(const RectTransform& trs, int index);
	virtual ~UI_FadeBase();

	void Update() override;
	virtual void Draw() {}

	void StartFadeOut();
	void StartFadeIn();

	bool IsFadeEnd() const;
protected:
	int m_CharaIndex;
private:
	enum FadeState
	{
		st_NONE = 0,
		st_FADEOUT,
		st_FADEIN,
	};
	FadeState m_FadeState;
	float m_FadeTime;
	virtual void fadeIn(float rate) {}
	virtual void fadeOut(float rate) {}
};

/// <summary>
/// 暗転するフェード
/// </summary>
class UI_FadeBlack : public UI_FadeBase
{
public:
	UI_FadeBlack();
	UI_FadeBlack(const RectTransform& trs, int index);
	~UI_FadeBlack();

	void Draw() override;
private:
	int m_Alpha;

	void fadeIn(float rate) override;
	void fadeOut(float rate) override;
};

