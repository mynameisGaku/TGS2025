#pragma once
#include <windows.h>

class TitleUIGridCursor
{
public:
	TitleUIGridCursor() : m_IndexX(0), m_IndexY(0) {}
	TitleUIGridCursor(const int _defaultX, const int _defaultY);
	
	inline void MoveTo(const int x, const int y)
	{
			m_IndexX = x;
			m_IndexY = y;
	}
	inline void Move(const int dx, const int dy)
	{
			m_IndexX += dx;
			m_IndexY += dy;
	}

	const int IndexX() const { return m_IndexX; }
	const int IndexY() const { return m_IndexY; }

private:
	int m_IndexX;
	int m_IndexY;
};