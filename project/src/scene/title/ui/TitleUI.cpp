#include "TitleUI.h"
#include "TitleUICanvas.h"
#include "TitleUIGridCursor.h"
#include <src/util/input/InputManager.h>
#include <src/util/input/MouseController.h>

void TitleUI::Update()
{
	if (not m_IsVisible)
		return;

	switch (m_Collision)
	{
		case TUI_COLLISION_NONE:
			// ‰½‚à‚µ‚È‚¢
			break;
		case TUI_COLLISION_RECT:
		{
			const Vector2 _pos = Vector2(m_GraphPosX, m_GraphPosY);
			const Vector2 _size = Vector2(m_GraphDestX, m_GraphDestY);
			switch (InputManager::GetLastInputDevice())
			{
				case KeyDefine::DeviceType::Mouse:
				{
					if (MouseController::ColCheck_CursorToBox(_pos, _size))
					{
						m_IsHover = true;
					}
				}
				case KeyDefine::DeviceType::Key:
				case KeyDefine::DeviceType::Pad:
				{
					if (m_pHolder)
					{
						auto gridCursor = m_pHolder->GetGridCursor();

						int indexX = gridCursor->IndexX();
						int indexY = gridCursor->IndexY();
					}
				}
				break;
			}
			break;
		}
		case TUI_COLLISION_CIRCLE:
		{
		}
	}
}

void TitleUI::Draw()
{
	if (not m_IsVisible)
		return;

	if (m_IsHolding)
		SetDrawBright(50, 50, 50);

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
		true);

	if (m_IsHolding)
		SetDrawBright(255, 255, 255);
}

void TitleUI::Release()
{

}

void TitleUI::Init(const UI_TITLE_DESC& desc, TitleUICanvas* pCanvas)
{
	m_GraphHandle			= desc.GRAPH_HANDLE;
	m_GraphPosX				= desc.GRAPH_POS_X;
	m_GraphPosY				= desc.GRAPH_POS_Y;
	m_GraphSrcX				= desc.GRAPH_SRC_X;
	m_GraphSrcY				= desc.GRAPH_SRC_Y;
	m_GraphDestX			= desc.GRAPH_DEST_X;
	m_GraphDestY			= desc.GRAPH_DEST_Y;
	for (auto& event : desc.EVENTS)
	{
		switch (event.TIMING)
		{
		case TUI_EVENT_TRIGGER_TIMING_HOLD:
			m_PressHoldEvents.push_back(event);
			break;
		case TUI_EVENT_TRIGGER_TIMING_ENTER:
			m_PressEnterEvents.push_back(event);
			break;
		case TUI_EVENT_TRIGGER_TIMING_RELEASE:
			m_PressReleaseEvents.push_back(event);
			break;
		default:
			break;
		}
	}
	m_Description			= desc.DESCRIPTION;
	m_IsActive				= false;
	m_IsVisible				= false;
	m_pHolder				= pCanvas;
}

void TitleUI::OnPressHold()
{

}

void TitleUI::OnPressEnter()
{

}

void TitleUI::OnPressRelease()
{

}
