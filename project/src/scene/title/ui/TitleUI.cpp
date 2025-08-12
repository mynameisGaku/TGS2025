#include "TitleUI.h"
#include "TitleUICanvas.h"
#include "TitleUIController.h"
#include "TitleUIGridCursor.h"
#include <src/util/input/InputManager.h>
#include <src/util/input/MouseController.h>
#include <src/util/file/resource_loader/resourceLoader.h>
#include <src/util/time/GameTime.h>
#include <src/reference/game/GameRef.h>
#include <algorithm>
#include <cmath>

static constexpr double DEG2RAD = 3.14159265358979323846 / 180.0;

void TitleUI::Update()
{
	if (not m_IsVisible)
		return;

	if (m_IsSelectable)
	{
		checkCursorCollision();

		if (m_IsHover)
		{
			if (InputManager::Push(KeyDefine::KeyCode::MouseButton1) ||
				InputManager::Push(KeyDefine::KeyCode::Z) ||
				InputManager::Push(KeyDefine::KeyCode::ButtonA))
			{
				OnPressEnter();
			}
			if (InputManager::Hold(KeyDefine::KeyCode::MouseButton1) ||
				InputManager::Hold(KeyDefine::KeyCode::Z) ||
				InputManager::Hold(KeyDefine::KeyCode::ButtonA))
			{
				OnPressHold();
			}
			if (InputManager::Release(KeyDefine::KeyCode::MouseButton1) ||
				InputManager::Release(KeyDefine::KeyCode::Z) ||
				InputManager::Release(KeyDefine::KeyCode::ButtonA))
			{
				OnPressRelease();
			}
		}
	}

	if (m_IsHover)
	{
		OnHover();
	}

	OnFrame();
}

void TitleUI::Draw()
{
	if (!m_IsVisible) return;

	const Vector2 anchor = m_pHolder->GetPoint(m_Anchor);

	const float sx = m_pHolder->ScaleX();
	const float sy = m_pHolder->ScaleY();
	const float s = m_LockAspect ? min(sx, sy) : sx;

	const float cx = (float)(m_GraphDestX * m_PivotX);
	const float cy = (float)(m_GraphDestY * m_PivotY);

	const float x = (float)(anchor.x + m_OffsetX_FromAnchor * sx);
	const float y = (float)(anchor.y + m_OffsetY_FromAnchor * sy);

	const int alpha = (int)std::round(std::clamp(m_Opacity, 0.0, 1.0) * 255.0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	const double rad = m_RotationDeg * DX_PI / 180.0;

	DrawRectRotaGraph2F(
		x, y,
		m_GraphSrcX, m_GraphSrcY,
		m_GraphDestX, m_GraphDestY,
		cx, cy,
		(float)(s * m_Scale), rad,
		(m_GraphHandle == -1 ? DX_NONE_GRAPH : m_GraphHandle),
		TRUE,
		FALSE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleUI::Release()
{
	m_IsActive = false;
	m_IsVisible = false;
	m_IsHolding = false;
	m_IsHover = false;
	m_PressEnterEvents.clear();
	m_PressHoldEvents.clear();
	m_PressReleaseEvents.clear();
}

void TitleUI::Init(const UI_TITLE_DESC& desc, TitleUICanvas* pCanvas)
{
	m_GraphHandle = ResourceLoader::LoadGraph(desc.GRAPH_PATH);
	m_OffsetX_FromAnchor = desc.OFFSET_X_FROM_ANCHOR;
	m_OffsetY_FromAnchor = desc.OFFSET_Y_FROM_ANCHOR;
	m_GraphSrcX = desc.GRAPH_SRC_X;
	m_GraphSrcY = desc.GRAPH_SRC_Y;
	m_GraphDestX = desc.GRAPH_DEST_X;
	m_GraphDestY = desc.GRAPH_DEST_Y;
	m_IndexX = desc.INDEX_X;
	m_IndexY = desc.INDEX_Y;

	m_IsActive = true;
	m_IsVisible = desc.IsVisible;
	m_IsHolding = false;
	m_IsHover = false;
	m_IsSelectable = desc.IS_SELECTABLE;

	m_RotationDeg = desc.RotationDeg;
	m_Scale = max(0.0001, desc.Scale);
	m_Opacity = std::clamp(desc.Opacity, 0.0, 1.0);
	m_ZIndex = desc.ZIndex;
	m_LockAspect = desc.LockAspectRatio;
	m_PivotX = std::clamp(desc.PivotX, 0.0, 1.0);
	m_PivotY = std::clamp(desc.PivotY, 0.0, 1.0);

	m_Name = desc.NAME;
	m_Description = desc.Description;
	m_Collision = desc.COLLISION;
	m_Anchor = desc.ANCHOR;
	m_pHolder = pCanvas;

	// イベント仕分け
	for (const auto& event : desc.EVENTS)
	{
		TUI_EVENT e = event;
		switch (e.Timing)
		{
		case TUI_EVENT_TRIGGER_TIMING_ENTER:
			m_PressEnterEvents.push_back(e);
			break;
		case TUI_EVENT_TRIGGER_TIMING_HOLD:
			m_PressHoldEvents.push_back(e);
			break;
		case TUI_EVENT_TRIGGER_TIMING_RELEASE:
			m_PressReleaseEvents.push_back(e);
			break;
		case TUI_EVENT_TRIGGER_TIMING_HOVER:
			m_HoverEvents.push_back(e);
			break;
		case TUI_EVENT_TRIGGER_TIMING_EVERY_FRAME:
			m_EveryFrameEvents.push_back(e);
			break;
		}
	}
}

void TitleUI::OnPressEnter()
{
	m_IsHolding = false;

	for (auto& event : m_PressEnterEvents)
	{
		m_pHolder->Controller()->TriggerEvent(event, event.Argument);
	}
}

void TitleUI::OnPressHold()
{
	m_IsHolding = true;

	for (auto& event : m_PressHoldEvents)
	{
		m_pHolder->Controller()->TriggerEvent(event, event.Argument);
	}
}

void TitleUI::OnPressRelease()
{
	m_IsHolding = false;

	for (auto& event : m_PressReleaseEvents)
	{
		m_pHolder->Controller()->TriggerEvent(event, event.Argument);
	}
}

void TitleUI::OnHover()
{
	for (auto& event : m_HoverEvents)
	{
		m_pHolder->Controller()->TriggerEvent(event, event.Argument);
	}
}

void TitleUI::OnFrame()
{
	for (auto& event : m_EveryFrameEvents)
	{
		m_pHolder->Controller()->TriggerEvent(event, event.Argument);
	}
}

void TitleUI::checkCursorCollision()
{
	m_IsHover = false;

	// アンカー＋オフセット位置（Pivotが置かれる位置）
	const Vector2 anchor = m_pHolder->GetPoint(m_Anchor);
	const double drawX = static_cast<double>(anchor.x) + static_cast<double>(m_OffsetX_FromAnchor);
	const double drawY = static_cast<double>(anchor.y) + static_cast<double>(m_OffsetY_FromAnchor);

	// 実表示サイズ（回転は無視してAABBで判定）
	const double w = static_cast<double>(m_GraphDestX) * m_Scale;
	const double h = static_cast<double>(m_GraphDestY) * m_Scale;

	// 左上（AABB）
	const double left = drawX - (static_cast<double>(m_GraphDestX) * m_PivotX) * m_Scale;
	const double top = drawY - (static_cast<double>(m_GraphDestY) * m_PivotY) * m_Scale;

	auto device = InputManager::GetLastInputDevice();
	switch (device)
	{
	case KeyDefine::DeviceType::Mouse:
	{
		switch (m_Collision)
		{
		case TUI_COLLISION_MODE_NONE:
			break;

		case TUI_COLLISION_MODE_RECT:
		{
			const Vector2 pos(static_cast<float>(left), static_cast<float>(top));
			const Vector2 size(static_cast<float>(w), static_cast<float>(h));
			if (MouseController::ColCheck_CursorToBox(pos, size))
			{
				m_IsHover = true;
			}
		}
		break;

		case TUI_COLLISION_MODE_CIRCLE:
		{
			const Vector2 center(static_cast<float>(left + w * 0.5), static_cast<float>(top + h * 0.5));
			const float radius = static_cast<float>(min(w, h) * 0.5);
			if (MouseController::ColCheck_CursorToCircle(center, radius))
			{
				m_IsHover = true;
			}
		}
		break;
		}

		if (m_IsHover)
		{
			m_pHolder->Controller()->GetGridCursor()->MoveTo(m_IndexX, m_IndexY);
		}
	}
	break;

	case KeyDefine::DeviceType::Key:
	case KeyDefine::DeviceType::Pad:
	{
		auto gridCursor = m_pHolder->Controller()->GetGridCursor();
		if (gridCursor->IndexX() == m_IndexX && gridCursor->IndexY() == m_IndexY)
		{
			m_IsHover = true;
		}
	}
	break;

	default:
		break;
	}
}
