#include "TitleUICanvas.h"
#include <src/common/setting/window/WindowSetting.h>
#include <src/util/logger/Logger.h>

void TitleUICanvas::Init(const UI_TITLE_CANVAS_DESC& desc, TitleUIController* pCon)
{
	m_pController	= pCon;
	m_Name			= desc.NAME;
	m_Begin			= desc.BEGIN;
	m_End			= desc.END;
	m_IsFitScreen	= desc.IS_FIT_SCREEN;
	m_IsActive		= desc.IS_DEFAULT_ACTIVATE;
}

void TitleUICanvas::Update()
{
	if (not m_IsActive)
		return;

	for (auto& ui : m_UIList)
	{
		ui.Update();
	}
}

void TitleUICanvas::Draw()
{
	if (not m_IsActive)
		return;

	for (auto& ui : m_UIList)
	{
		ui.Draw();
	}
}

void TitleUICanvas::Release()
{
	for (auto& ui : m_UIList)
	{
		ui.Release();
	}
	m_UIList.clear();
	m_IsActive = false;
	m_pController = nullptr;
	m_Name.clear();
}

TitleUI TitleUICanvas::GetUI(const std::string& name)
{
	for (auto& ui : m_UIList)
	{
		if (ui.GetName() == name)
		{
			return ui;
		}
	}
	return TitleUI();  // 見つからなかった場合は空のTitleUIを返す
}

Vector2 TitleUICanvas::GetPoint(const TUI_CANVAS_ANCHOR_POINT& anchor)
{
	Vector2 result{};

	Vector2 begin{};
	Vector2 end{};

	if (m_IsFitScreen)
	{
		// 画面全体にフィットする場合、画面サイズを取得
		float screenWidth	= WindowSetting::Inst().width;
		float screenHeight	= WindowSetting::Inst().height;
		begin				= Vector2(0.0f, 0.0f);
		end					= Vector2(screenWidth, screenHeight);
	}
	else
	{
		begin	= m_Begin;
		end		= m_End;
	}

	switch (anchor)
	{
	case TUI_CANVAS_ANCHOR_POINT_TOP_LEFT:
		result = begin;
		break;
	case TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT:
		result = Vector2(end.x, begin.y);
		break;
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT:
		result = Vector2(begin.x, end.y);
		break;
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT:
		result = end;
		break;
	case TUI_CANVAS_ANCHOR_POINT_CENTER:
		result = Vector2(begin.x + (end.x * 0.5f), begin.y + (end.y* 0.5f));
		break;
	case TUI_CANVAS_ANCHOR_POINT_LEFT:
		result = Vector2(begin.x, (begin.y + end.y) * 0.5f);
		break;
	case TUI_CANVAS_ANCHOR_POINT_RIGHT:
		result = Vector2(end.x, (begin.y + end.y) * 0.5f);
		break;
	case TUI_CANVAS_ANCHOR_POINT_TOP:
		result = Vector2(begin.x + (end.x * 0.5f), begin.y);
		break;
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM:
		result = Vector2(begin.x + (end.x * 0.5f), end.y);
		break;
	default:
		Logger::FormatErrorLog("Unknown anchor point: {}", static_cast<int>(anchor));
		result = Vector2::Zero;  // デフォルトはゼロベクトル
		break;
	}
	
	return result;
}

const Vector2 TitleUICanvas::GetBegin() const
{
	return m_Begin;
}

const Vector2 TitleUICanvas::GetEnd() const
{
	return m_End;
}
