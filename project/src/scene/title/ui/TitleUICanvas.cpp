#include "TitleUICanvas.h"
#include "TitleUIController.h"
#include "TitleUIGridCursor.h"
#include <src/common/setting/window/WindowSetting.h>
#include <src/util/logger/Logger.h>
#include <vector>
#include <algorithm>
#include <src/util/input/InputManager.h>
#include <src/util/sound/SoundManager.h>

void TitleUICanvas::Init(const UI_TITLE_CANVAS_DESC& desc, TitleUIController* pCon)
{
	m_pController = pCon;
	m_Name = desc.NAME;
	m_Begin = desc.BEGIN;
	m_End = desc.END;
	m_IsFitScreen = desc.IS_FIT_SCREEN;
	m_IsActive = desc.IS_DEFAULT_ACTIVATE;
	m_PrevCanvas = desc.PREV_NAME;
}

void TitleUICanvas::Update()
{
	if (not m_IsActive)
		return;

	if (m_IsFirstUpdate)
	{
		m_pController->GetGridCursor()->Activate();

		m_IsFirstUpdate = false;
	}

	for (auto& ui : m_UIList)
	{
		ui.Update();
	}

	// カーソルの位置を制限
	auto cursor = m_pController->GetGridCursor();
	if(cursor->IndexX() > m_EndIndexX)
		cursor->MoveTo(m_EndIndexX, cursor->IndexY());
	if (cursor->IndexY() > m_EndIndexY)
		cursor->MoveTo(cursor->IndexX(), m_EndIndexY);
	if (cursor->IndexX() < m_BeginIndexX)
		cursor->MoveTo(m_BeginIndexX, cursor->IndexY());
	if (cursor->IndexY() < m_BeginIndexY)
		cursor->MoveTo(cursor->IndexX(), m_BeginIndexY);

	if (InputManager::Push("Cancel"))
	{
		if (not m_PrevCanvas.empty()) 
		{
			m_pController->activateCanvas(m_PrevCanvas);
			SoundManager::Play("SE_menu_click.mp3", "SE_menu_click.mp3");
		}
	}

}

void TitleUICanvas::Draw()
{
	if (not m_IsActive)
		return;

	// ZIndex でソートして描画
	std::vector<std::pair<int, TitleUI*>> sorted;
	sorted.reserve(m_UIList.size());
	int order = 0;
	for (auto& ui : m_UIList)
	{
		if (ui.IsVisible())
		{
			sorted.emplace_back(order++, &ui);
		}
	}

	std::stable_sort(sorted.begin(), sorted.end(),
		[](const auto& a, const auto& b)
		{
			// ZIndex 昇順
			const int za = a.second->GetZIndex();
			const int zb = b.second->GetZIndex();
			return za < zb;
		});

	for (auto& p : sorted)
	{
		p.second->Draw();
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

void TitleUICanvas::AddUI(const TitleUI& ui)
{
	if (ui.GetIndexX() < m_BeginIndexX)
		m_BeginIndexX = ui.GetIndexX();
	if (ui.GetIndexY() < m_BeginIndexY)
		m_BeginIndexY = ui.GetIndexY();
	if(ui.GetIndexX() > m_EndIndexX)
		m_EndIndexX = ui.GetIndexX();
	if (ui.GetIndexY() > m_EndIndexY)
		m_EndIndexY = ui.GetIndexY();

	m_UIList.push_back(ui);
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
	return TitleUI();
}

Vector2 TitleUICanvas::GetPoint(const TUI_CANVAS_ANCHOR_POINT& anchor)
{
	Vector2 origin; float sx, sy;
	calcMapping(origin, sx, sy);

	const Vector2 size = CanvasDesignSize();

	auto at = [&](float nx, float ny) -> Vector2
		{
			// nx, ny は 0~1 の正規化アンカー（左上(0,0)~右下(1,1)）
			return Vector2(origin.x + size.x * nx * sx,
				origin.y + size.y * ny * sy);
		};

	switch (anchor)
	{
	case TUI_CANVAS_ANCHOR_POINT_TOP_LEFT:      return at(0.0f, 0.0f);
	case TUI_CANVAS_ANCHOR_POINT_TOP:           return at(0.5f, 0.0f);
	case TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT:     return at(1.0f, 0.0f);
	case TUI_CANVAS_ANCHOR_POINT_LEFT:          return at(0.0f, 0.5f);
	case TUI_CANVAS_ANCHOR_POINT_CENTER:        return at(0.5f, 0.5f);
	case TUI_CANVAS_ANCHOR_POINT_RIGHT:         return at(1.0f, 0.5f);
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT:   return at(0.0f, 1.0f);
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM:        return at(0.5f, 1.0f);
	case TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT:  return at(1.0f, 1.0f);
	default:                                    return at(0.5f, 0.5f);
	}
}

const Vector2 TitleUICanvas::GetBegin() const
{
	return m_Begin;
}

const Vector2 TitleUICanvas::GetEnd() const
{
	return m_End;
}
Vector2 TitleUICanvas::CanvasDesignSize() const
{
	return Vector2(m_End.x - m_Begin.x, m_End.y - m_Begin.y);
}

void TitleUICanvas::calcMapping(Vector2& origin, float& sx, float& sy) const
{
	const Vector2 design = CanvasDesignSize();

	if (!m_IsFitScreen)
	{
		origin = m_Begin;
		sx = 1.0f;
		sy = 1.0f;
		return;
	}

	// 画面にフィット（アスペクト維持・レターボックス）
	const float sw = (float)WindowSetting::Inst().width;
	const float sh = (float)WindowSetting::Inst().height;

	const float sxRaw = sw / design.x;
	const float syRaw = sh / design.y;
	const float s = min(sxRaw, syRaw);   // 等倍スケール

	const float vw = design.x * s;
	const float vh = design.y * s;

	origin = Vector2((sw - vw) * 0.5f, (sh - vh) * 0.5f); // 画面中央に配置
	sx = s;
	sy = s;
}

float TitleUICanvas::ScaleX() const
{
	Vector2 o; float sx, sy;
	const_cast<TitleUICanvas*>(this)->calcMapping(o, sx, sy);
	return sx;
}

float TitleUICanvas::ScaleY() const
{
	Vector2 o; float sx, sy;
	const_cast<TitleUICanvas*>(this)->calcMapping(o, sx, sy);
	return sy;
}

Vector2 TitleUICanvas::CanvasOriginOnScreen() const
{
	Vector2 o; float sx, sy;
	const_cast<TitleUICanvas*>(this)->calcMapping(o, sx, sy);
	return o;
}