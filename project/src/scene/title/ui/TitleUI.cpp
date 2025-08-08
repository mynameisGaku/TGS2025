#include "TitleUI.h"
#include "TitleUICanvas.h"
#include "TitleUIController.h"
#include "TitleUIGridCursor.h"
#include <src/util/input/InputManager.h>
#include <src/util/input/MouseController.h>
#include <src/util/file/resource_loader/resourceLoader.h>
#include <src/util/time/GameTime.h>
#include <src/reference/game/GameRef.h>
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
	if (!m_IsVisible)
		return;

	if (m_IsHover)
	{
		SetDrawBright(255, 180, 180); // ホバー時色変更（例）
	}
	else if (m_IsHolding)
	{
		SetDrawBright(100, 100, 100); // 押下中色
	}

	Vector2 point = m_pHolder->GetPoint(m_Anchor);

	DrawRectRotaGraph(
		(int)point.x + m_OffsetX_FromAnchor,
		(int)point.y + m_OffsetY_FromAnchor,
		m_GraphSrcX,
		m_GraphSrcY,
		m_GraphDestX,
		m_GraphDestY,
		1.0,
		0.0,
		m_GraphHandle == -1 ? DX_NONE_GRAPH : m_GraphHandle,
		true
	);

	SetDrawBright(255, 255, 255); // 明度リセット
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
	m_GraphHandle			= ResourceLoader::LoadGraph(desc.GRAPH_PATH);	// 画像のハンドル
	m_OffsetX_FromAnchor	= desc.OFFSET_X_FROM_ANCHOR;						// 描画開始位置 x
	m_OffsetY_FromAnchor	= desc.OFFSET_Y_FROM_ANCHOR;						// 描画開始位置 y
	m_GraphSrcX				= desc.GRAPH_SRC_X;									// 画像切り抜き開始位置 x
	m_GraphSrcY				= desc.GRAPH_SRC_Y;									// 画像切り抜き開始位置 y
	m_GraphDestX			= desc.GRAPH_DEST_X;								// 画像切り抜き終了位置 x
	m_GraphDestY			= desc.GRAPH_DEST_Y;								// 画像切り抜き終了位置 y
	m_IndexX				= desc.INDEX_X;										// Canvas上でのIndex x
	m_IndexY				= desc.INDEX_Y;										// Canvas上でのIndex y
	m_IsActive				= true;												// 有効か？
	m_IsVisible				= true;												// 表示されているか？
	m_IsHolding				= false;											// 押下されているか？
	m_IsHover				= false;											// カーソルがこのUIの上にあるか？
	m_IsSelectable			= desc.IS_SELECTABLE;								// このUIが選択可能か？
	m_pHolder				= pCanvas;
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

	for( auto& event : m_PressHoldEvents)
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
	m_IsHover = false;  // 状態リセット

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
			static const Vector2 _pos(m_OffsetX_FromAnchor, m_OffsetY_FromAnchor);
			static const Vector2 _size(m_GraphDestX, m_GraphDestY);

			if (MouseController::ColCheck_CursorToBox(_pos, _size))
			{
				m_IsHover = true;
			}
		}
		break;
		case TUI_COLLISION_MODE_CIRCLE:
		{
			Vector2 center(m_OffsetX_FromAnchor, m_OffsetY_FromAnchor);
			float radius = (float)(min(m_GraphDestX, m_GraphDestY) / 2);
			if (MouseController::ColCheck_CursorToCircle(center, radius))
			{
				m_IsHover = true;
			}
		}
		break;
		}

		if (m_IsHover)
			m_pHolder->Controller()->GetGridCursor()->MoveTo(m_IndexX, m_IndexY);
	}
	case KeyDefine::DeviceType::Key:
	case KeyDefine::DeviceType::Pad:
	{
		// Pad or Keyの場合、Indexを照合するだけでいい
		auto gridCursor = m_pHolder->Controller()->GetGridCursor();
		if (gridCursor->IndexX() == m_IndexX && gridCursor->IndexY() == m_IndexY)
		{
			m_IsHover = true;
		}
	}
	}
}
