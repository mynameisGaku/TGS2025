#pragma once
#include <windows.h>

#include <src/util/math/Vector2.h>

class TitleUIGridCursor
{
public:
	TitleUIGridCursor() : m_IndexX(0), m_IndexY(0) {}
	TitleUIGridCursor(const int _defaultX, const int _defaultY);

	void Update();
	void Draw();

	void MoveTo(int x, int y);

	void Move(int dx, int dy);

	const int IndexX() const { return m_IndexX; }
	const int IndexY() const { return m_IndexY; }

	void SetPos(const Vector2& pos) { m_Position = pos; }
	void SetOffset(const Vector2& offset) { m_Offset = offset; }
	void SetImage(int handle) { m_hImage = handle; }

private:
	int m_IndexX{};
	int m_IndexY{};
	int m_hImage{};
	int m_FrameCounter{};

	Vector2 m_Position{};
	Vector2 m_Offset{};
	Vector2 m_SinCurve{};
};