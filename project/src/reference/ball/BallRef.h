#pragma once
#include <string>
#include "src/util/math/Vector3.h"

/// <summary>
/// ボールのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class BallRef
{
private:
	const std::string PATH    = "data/Json/Ball/Ball.json";
	const std::string FILEKEY = "Ball";

	static BallRef* instance;
public:
	static BallRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new BallRef;
		}
		return instance;
	}

	// " リファレンス解放 "
	// ↑を文字列検索したら着くところで解放してます。(おそらくメインループのすぐあと)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
	}

	void Load();

	Vector3 GravityDefault;		// デフォルトの重力(cm/s^2)
	Vector3 FrictionDefault;	// デフォルトの摩擦係数
	float	BouncinessDefault = 0.0f;	// デフォルトの反発係数
	float	SpeedDefault = 0.0f;
	float	HomingTimeMax = 0.0f;
	float	LifeTimeMax = 0.0f;
	int		Max = 0;
};

#define BALL_REF (*BallRef::Inst())
