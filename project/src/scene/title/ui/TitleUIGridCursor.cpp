#include "TitleUIGridCursor.h"
#include <src/util/input/InputManager.h>

TitleUIGridCursor::TitleUIGridCursor(const int _defaultX, const int _defaultY) 
	:
	m_IndexX(_defaultX)
	,m_IndexY(_defaultY)
{
}

void TitleUIGridCursor::MoveTo(int x, int y)
{
	m_IndexX = x;
	m_IndexY = y;
}

void TitleUIGridCursor::Move(int dx, int dy)
{
	m_IndexX += dx;
	m_IndexY += dy;
}

void TitleUIGridCursor::Update()
{
	if (InputManager::Push(KeyDefine::KeyCode::MouseButton1) ||
		InputManager::Push(KeyDefine::KeyCode::Z) ||
		InputManager::Push(KeyDefine::KeyCode::ButtonA) ||
		InputManager::Hold(KeyDefine::KeyCode::MouseButton1) ||
		InputManager::Hold(KeyDefine::KeyCode::Z) ||
		InputManager::Hold(KeyDefine::KeyCode::ButtonA)||
		InputManager::Release(KeyDefine::KeyCode::MouseButton1) ||
		InputManager::Release(KeyDefine::KeyCode::Z) ||
		InputManager::Release(KeyDefine::KeyCode::ButtonA))
	{
		return;
	}

	auto device = InputManager::GetLastInputDevice();
	switch (device)
	{
		case KeyDefine::DeviceType::Mouse:
		{
			// マウスの場合はとくに何もしない。
		}
		break;
		case KeyDefine::DeviceType::Key:
		case KeyDefine::DeviceType::Pad:
		{
			if (InputManager::Push(KeyDefine::KeyCode::LeftArrow) ||
				InputManager::Push(KeyDefine::KeyCode::Left))
			{
				Move(-1, 0);
			}
			if (InputManager::Push(KeyDefine::KeyCode::RightArrow) ||
				InputManager::Push(KeyDefine::KeyCode::Right))
			{
				Move(1, 0);
			}
			if (InputManager::Push(KeyDefine::KeyCode::UpArrow) ||
				InputManager::Push(KeyDefine::KeyCode::Up))
			{
				Move(0, -1);
			}
			if (InputManager::Push(KeyDefine::KeyCode::DownArrow) ||
				InputManager::Push(KeyDefine::KeyCode::Down))
			{
				Move(0, 1);
			}
		}
		break;
		default:
			break;
	}
}

void TitleUIGridCursor::Draw()
{
	if (m_hImage == -1)
		return;
	const int x = m_Position.x + m_Offset.x;
	const int y = m_Position.y + m_Offset.y;
	DrawGraph(x, y, m_hImage, TRUE);
}