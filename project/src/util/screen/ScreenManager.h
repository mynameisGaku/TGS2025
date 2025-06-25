#pragma once
#include "src/util/math/Vector2.h"

namespace ScreenManager
{
	void Init();

	int GetScreenDivCount();

	int GetScreenDivWidth();
	int GetScreenDivHeight();

	Vector2 GetScreenBeginPos(int divCount);
	Vector2 GetScreenEndPos(int divCount);
}