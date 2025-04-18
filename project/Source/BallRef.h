#pragma once
#include "Util/Vector3.h"

/// <summary>
/// ボールのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class BallRef
{
public:
	static BallRef& Inst()
	{
		static BallRef instance;
		return instance;
	}

	void Load();

	Vector3 GravityDefault;		// デフォルトの重力(cm/s^2)
	Vector3 FrictionDefault;	// デフォルトの摩擦係数
	float BouncinessDefault = 0.0f;	// デフォルトの反発係数
	float SpeedDefault;
};

#define BALL_REF BallRef::Inst()
