#pragma once
#include "src/util/math/Vector3.h"
#include <string>

// �e��X�|�i�[�̐ݒ���

/// <summary>
/// �{�[���X�|�i�[�̍\�����
/// </summary>
/// <author>���{��</author>
struct BALL_SPAWNER_DESC
{
    float       INTERVAL_SEC                    = 0.0f;             // �X�|�[���Ԋu(�b)
    float       INTERVAL_SEC_RANDOM_RANGE       = 0.0f;             // �X�|�[���Ԋu�̃����_���͈�(�b)
    int         SPAWN_AMOUNT_ONCE_MAX           = 0;                // ��x�ɃX�|�[������ő吔
    int         SPAWN_AMOUNT_ONCE_MIN           = 0;                // ��x�ɃX�|�[������ŏ���
    int         SPAWN_AMOUNT_ONCE_RANDOM_RANGE  = 0;                // ��x�ɃX�|�[�����鐔�̃����_���͈�
    int         SPAWN_AMOUNT_INITIAL            = 0;                // �����X�|�[����
    Vector3     SPAWN_RANGE                     = Vector3::Zero;    // �X�|�[���ʒu�͈̔�
    Vector3     SPAWN_INITIAL_VELOCITY          = Vector3::Zero;    // �X�|�[�����̏������x
};

/// <summary>
/// �L�����X�|�i�[�̍\�����
/// </summary>
/// <author>���{��</author>
struct CHARA_SPAWN_POINT_DESC
{
    Vector3     SPAWN_INITIAL_VELOCITY          = Vector3::Zero;    // �X�|�[�����̏������x
};