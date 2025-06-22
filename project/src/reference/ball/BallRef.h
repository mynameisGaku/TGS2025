#pragma once
#include <string>
#include "src/util/math/Vector3.h"
#include "src/util/singleton/singleton.h"
#include <vector>

class BallThrowParam
{
public:
	int ChargeLevel;
	float Speed;
	float EffectScale;
	float EffectSpeed;
};


/// <summary>
/// ボールのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class BallRef
{
public:
	BallRef() { Load(); }
	void Load();

	Vector3 Gravity;		// 重力(cm/s^2)
	Vector3 Friction;		// 摩擦係数
	float	Bounciness;		// 反発係数
	std::vector<BallThrowParam> ChargeLevels;	// ボールのチャージ段階とその飛翔パラメータ

	float	DropForce_Vertical;
	float	DropForce_Horizontal;

	float	LifeTimeMax;
	int		PoolMax;
};

#define BALL_REF Singleton<BallRef>::Instance()
