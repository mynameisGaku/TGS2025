#pragma once
#include <DxLib.h>
#include <windows.h>
#include <list>
#include <functional>
#include <string>
#include <vendor/nlohmann/json.hpp>

/// <summary>
/// イベントがトリガーするタイミング
/// </summary>
enum TUI_EVENT_TRIGGER_TIMING
{
	TUI_EVENT_TRIGGER_TIMING_HOLD,
	TUI_EVENT_TRIGGER_TIMING_ENTER,
	TUI_EVENT_TRIGGER_TIMING_RELEASE,
	TUI_EVENT_TRIGGER_TIMING_HOVER,
	TUI_EVENT_TRIGGER_TIMING_EVERY_FRAME,
};

/// <summary>
/// 当たり判定方法
/// </summary>
enum TUI_COLLISION_MODE
{
	TUI_COLLISION_MODE_NONE,
	TUI_COLLISION_MODE_CIRCLE,
	TUI_COLLISION_MODE_RECT,
};

enum TUI_CANVAS_ANCHOR_POINT
{
	TUI_CANVAS_ANCHOR_POINT_CENTER = 0,
	TUI_CANVAS_ANCHOR_POINT_LEFT,
	TUI_CANVAS_ANCHOR_POINT_RIGHT,
	TUI_CANVAS_ANCHOR_POINT_TOP,
	TUI_CANVAS_ANCHOR_POINT_TOP_LEFT,
	TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT,
};

/// <summary>
/// イベント一つのまとまり
/// </summary>
struct TUI_EVENT
{
	TUI_EVENT_TRIGGER_TIMING	Timing{};
	std::string					Event{};
	nlohmann::json				Argument{};
	float						Counter{};
	float						Duration{};
	std::string					Description{};
};

/// <summary>
/// タイトルUIの記述
/// </summary>
struct UI_TITLE_DESC
{
	std::string				GRAPH_PATH{};
	int						OFFSET_X_FROM_ANCHOR{};
	int						OFFSET_Y_FROM_ANCHOR{};
	int						GRAPH_SRC_X{};
	int						GRAPH_SRC_Y{};
	int						GRAPH_DEST_X{};
	int						GRAPH_DEST_Y{};
	int						INDEX_X{};
	int						INDEX_Y{};
	bool					IS_SELECTABLE{};
	std::list<TUI_EVENT>	EVENTS{};
	std::string				Description{};
	TUI_COLLISION_MODE		COLLISION{};
	std::string				NAME{};
	TUI_CANVAS_ANCHOR_POINT ANCHOR{};
	bool					IsVisible{ true };
	double					RotationDeg{ 0.0 };
	double					Scale{ 1.0 };
	double					Opacity{ 1.0 };
	int						ZIndex{ 0 };
	bool					LockAspectRatio{ true };
	double					PivotX{ 0.5 };
	double					PivotY{ 0.5 };
};

class TitleUICanvas;

/// <summary>
/// タイトルUI
/// </summary>
class TitleUI
{
public:
	~TitleUI() {}

	void	Update();
	void	Draw();
	void	Release();

	void	Init(const UI_TITLE_DESC& desc, TitleUICanvas* pCanvas);

	void	OnPressHold();
	void	OnPressEnter();
	void	OnPressRelease();
	void	OnHover();
	void	OnFrame();

	int			GetZIndex() const { return m_ZIndex; }
	bool		IsVisible() const { return m_IsVisible; }
	std::string	GetName()   const { return m_Name; }

private:
	void checkCursorCollision();

	int								m_GraphHandle{};
	int								m_OffsetX_FromAnchor{};
	int								m_OffsetY_FromAnchor{};
	int								m_GraphSrcX{};
	int								m_GraphSrcY{};
	int								m_GraphDestX{};
	int								m_GraphDestY{};
	int								m_IndexX{};
	int								m_IndexY{};
	bool							m_IsActive{};
	bool							m_IsVisible{};
	bool							m_IsHolding{};
	bool							m_IsHover{};
	bool							m_IsSelectable{};
	bool							m_IsEventTriggered{};
	std::string						m_Name{};
	std::string						m_Description{};
	std::list<TUI_EVENT>			m_PressHoldEvents{};
	std::list<TUI_EVENT>			m_PressEnterEvents{};
	std::list<TUI_EVENT>			m_PressReleaseEvents{};
	std::list<TUI_EVENT>			m_HoverEvents{};
	std::list<TUI_EVENT>			m_EveryFrameEvents{};
	TUI_COLLISION_MODE				m_Collision{};
	TUI_CANVAS_ANCHOR_POINT			m_Anchor{};
	TitleUICanvas* m_pHolder{};
	double							m_RotationDeg{ 0.0 };
	double							m_Scale{ 1.0 };
	double							m_Opacity{ 1.0 };
	int								m_ZIndex{ 0 };
	bool							m_LockAspect{ true };
	double							m_PivotX{ 0.5 };
	double							m_PivotY{ 0.5 };
};
