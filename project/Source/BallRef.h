#pragma once
#include <string>
#include "Util/Vector3.h"

/// <summary>
/// ボールのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class BallRef
{
private:
	const std::string PATH    = "data/Json/Ball.json";
	const std::string FILEKEY = "Ball";
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
