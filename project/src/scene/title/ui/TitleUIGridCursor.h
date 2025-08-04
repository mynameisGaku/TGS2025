#pragma once
#include <windows.h>

class TitleUIGridCursor
{
public:
	TitleUIGridCursor() : m_IndexX(0), m_IndexY(0) {}
	TitleUIGridCursor(const int _defaultX, const int _defaultY);

	void Update();

	void MoveTo(int x, int y);

	void Move(int dx, int dy);

	const int IndexX() const { return m_IndexX; }
	const int IndexY() const { return m_IndexY; }

private:
	int m_IndexX;
	int m_IndexY;
};