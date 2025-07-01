#pragma once
#include "src/util/singleton/singleton.h"

/// <summary>
/// �}�b�`�}�l�[�W���[��Json�p�����[�^��ǂݍ���
/// </summary>
/// <author>�����h�l</author>
class MatchRef
{
public:
	MatchRef() { Load(); }
	void Load();

	float GameOver_Duration;
};

#define MATCH_REF Singleton<MatchRef>::Instance()
