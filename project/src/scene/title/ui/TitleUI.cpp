#include "TitleUI.h"
#include "TitleUICanvas.h"
#include "TitleUIController.h"
#include "TitleUIGridCursor.h"
#include <src/util/input/InputManager.h>
#include <src/util/input/MouseController.h>

void TitleUI::Update()
{
	if (not m_IsVisible)
		return;

	m_IsHover = false;  // 状態リセット

	const Vector2 _pos(m_GraphPosX, m_GraphPosY);
	const Vector2 _size(m_GraphDestX, m_GraphDestY);

	switch (m_Collision)
	{
	case TUI_COLLISION_MODE_NONE:
		break;

	case TUI_COLLISION_MODE_RECT:
	{
		auto device = InputManager::GetLastInputDevice();
		switch (device)
		{
		case KeyDefine::DeviceType::Mouse:
		{
			if (MouseController::ColCheck_CursorToBox(_pos, _size))
			{
				m_IsHover = true;
				auto gridCursor = m_pHolder->Controller()->GetGridCursor();
				gridCursor->MoveTo(m_IndexX, m_IndexY);

				if (InputManager::Release(KeyDefine::KeyCode::MouseLeft))
				{
					m_IsHolding = true;
					OnPressRelease();
					for (const auto& event : m_PressReleaseEvents)
					{
						m_pHolder->Controller()->TriggerEvent(event);
					}
				}
			}
			break;
		}
		case KeyDefine::DeviceType::Key:
		case KeyDefine::DeviceType::Pad:
		{
			auto gridCursor = m_pHolder->Controller()->GetGridCursor();
			int indexX = gridCursor->IndexX();
			int indexY = gridCursor->IndexY();

			if (m_IndexX == indexX && m_IndexY == indexY)
			{
				m_IsHover = true;

				if (InputManager::Push(KeyDefine::KeyCode::Enter) ||
					InputManager::Push(KeyDefine::KeyCode::Z))
				{
					m_IsHolding = true;
					OnPressEnter();
					for (const auto& event : m_PressEnterEvents)
					{
						m_pHolder->Controller()->TriggerEvent(event);
					}
				}
				else if (InputManager::Push(KeyDefine::KeyCode::Space))
				{
					m_IsHolding = true;
					OnPressHold();
					for (const auto& event : m_PressHoldEvents)
					{
						m_pHolder->Controller()->TriggerEvent(event);
					}
				}
				else if (InputManager::Release(KeyDefine::KeyCode::X))
				{
					OnPressRelease();
					for (const auto& event : m_PressReleaseEvents)
					{
						m_pHolder->Controller()->TriggerEvent(event);
					}
				}
			}
			else
			{
				m_IsHover = false;
			}
			break;
		}
		}
		break;
	}

	case TUI_COLLISION_MODE_CIRCLE:
	{
		Vector2 center(m_GraphPosX, m_GraphPosY);
		float radius = (float)(std::min(m_GraphDestX, m_GraphDestY) / 2);
		if (MouseController::ColCheck_CursorToCircle(center, radius))
		{
			m_IsHover = true;
			auto gridCursor = m_pHolder->Controller()->GetGridCursor();
			gridCursor->MoveTo(m_IndexX, m_IndexY);

			if (InputManager::Release(KeyDefine::KeyCode::MouseLeft))
			{
				m_IsHolding = true;
				OnPressRelease();
				for (const auto& event : m_PressReleaseEvents)
				{
					m_pHolder->Controller()->TriggerEvent(event);
				}
			}
		}
		break;
	}
	}
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

	DrawRectRotaGraph(
		m_GraphPosX,
		m_GraphPosY,
		m_GraphSrcX,
		m_GraphSrcY,
		m_GraphDestX,
		m_GraphDestY,
		1.0,
		0.0,
		m_GraphHandle,
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

void TitleUI::OnPressHold()
{
	for( auto& event : m_PressHoldEvents)
	{
		TUI_EVENT e = event;
		m_pHolder->Controller()->TriggerEvent(e.EVENT);
	}
}

void TitleUI::OnPressEnter()
{
}

void TitleUI::OnPressRelease()
{
	m_IsHolding = false;
}
