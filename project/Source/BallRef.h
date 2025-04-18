#pragma once
#include "Util/Vector3.h"

/// <summary>
/// �{�[����Json�p�����[�^��ǂݍ���
/// </summary>
/// <author>�����h�l</author>
class BallRef
{
public:
	static BallRef& Inst()
	{
		static BallRef instance;
		return instance;
	}

	void Load();

	Vector3 GravityDefault;		// �f�t�H���g�̏d��(cm/s^2)
	Vector3 FrictionDefault;	// �f�t�H���g�̖��C�W��
	float BouncinessDefault = 0.0f;	// �f�t�H���g�̔����W��
	float SpeedDefault;
};

#define BALL_REF BallRef::Inst()
