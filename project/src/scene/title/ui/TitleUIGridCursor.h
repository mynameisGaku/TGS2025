#pragma once
#include <windows.h>

class TitleUIGridCursor
{
public:

	void Update();


	const int IndexX() const { return m_IndexX; }
	const int IndexY() const { return m_IndexY; }

private:
	int m_IndexX;
	int m_IndexY;
};