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
	std::vector<BallThrowParam> ChargeLevels;	// �{�[���̃`���[�W�i�K�Ƃ��̔��ăp�����[�^

	float	DropForce_Vertical;
	float	DropForce_Horizontal;

	float	LifeTimeMax;
	int		PoolMax;
};

#define BALL_REF Singleton<BallRef>::Instance()
