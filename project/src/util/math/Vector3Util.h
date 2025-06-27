#pragma once
#include "src/util/math/Vector3.h"
class Vector3Util
{
public:
	// 2次元ベクトルの角度
	static float Vec2ToRad(float x, float y)
	{
		return atan2f(y, x);
	}

	// 方向ベクトルの回転をオイラー角に変換する
	static Vector3 DirToEuler(const Vector3& dir)
	{
		Vector3 euler;

		euler.x = Vec2ToRad(Vector3(dir.x, 0.0f, dir.z).GetLength(), dir.y);
		euler.y = Vec2ToRad(dir.z, dir.x);
		euler.z = 0;

		return euler;
	}

	// 二つのベクトルの角度を求める
	static float AngleBetween(const Vector3& a, const Vector3& b)
	{
		const float dot = Vector3::Dot(a, b);

		const float lenA = a.GetLength();
		if (lenA == 0)
			return 0.0f;
		const float lenB = b.GetLength();
		if (lenB == 0)
			return 0.0f;

		return acosf(dot / (lenA * lenB));
	}
};
