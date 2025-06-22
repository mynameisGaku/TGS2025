#pragma once
#include <string>
#include "src/util/math/Vector3.h"
#include "src/util/singleton/singleton.h"

/// <summary>
/// �{�[����Json�p�����[�^��ǂݍ���
/// </summary>
/// <author>�����h�l</author>
class BallRef
{
public:
	BallRef() { Load(); }
	void Load();

	Vector3 Gravity;		// �d��(cm/s^2)
	Vector3 Friction;		// ���C�W��
	float	Bounciness;		// �����W��
	float	SpeedDefault;	// ���ߖ����̑��x

	float	DropForce_Vertical;
	float	DropForce_Horizontal;

	float	LifeTimeMax;
	int		PoolMax;
};

#define BALL_REF Singleton<BallRef>::Instance()
