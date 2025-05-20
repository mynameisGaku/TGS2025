#pragma once
#include <string>
#include "src/util/math/Vector3.h"

/// <summary>
/// �{�[����Json�p�����[�^��ǂݍ���
/// </summary>
/// <author>�����h�l</author>
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

	// " ���t�@�����X��� "
	// ���𕶎��񌟍������璅���Ƃ���ŉ�����Ă܂��B(�����炭���C�����[�v�̂�������)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
	}

	void Load();

	Vector3 GravityDefault;		// �f�t�H���g�̏d��(cm/s^2)
	Vector3 FrictionDefault;	// �f�t�H���g�̖��C�W��
	float	BouncinessDefault = 0.0f;	// �f�t�H���g�̔����W��
	float	SpeedDefault = 0.0f;
	float	HomingTimeMax = 0.0f;
	float	LifeTimeMax = 0.0f;
	int		Max = 0;
};

#define BALL_REF (*BallRef::Inst())
