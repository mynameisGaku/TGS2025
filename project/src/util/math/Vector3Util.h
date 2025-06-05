#pragma once
#include "src/util/math/Vector3.h"
class Vector3Util
{
public:
	static float Vec2ToRad(float x, float y)
	{
		return atan2f(y, x);
	}

	static Vector3 DirToEuler(const Vector3& dir)
	{
		Vector3 euler;

		euler.x = Vec2ToRad(Vector3(dir.x, 0.0f, dir.z).GetLength(), dir.y);
		euler.y = Vec2ToRad(dir.z, dir.x);
		euler.z = 0;

		return euler;
	}
};
