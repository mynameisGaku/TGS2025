#pragma once
#include "src/util/math/Vector3.h"
#include <string>

/// <summary>
/// �X�|�i�[�̍\�����
/// </summary>
struct SPAWNER_DESC
{
    float       INTERVAL_SEC                    = 0.0f;
    float       INTERVAL_SEC_RANDOM_RANGE       = 0.0f;
    int         SPAWN_AMOUNT_ONCE_MAX           = 0;
    int         SPAWN_AMOUNT_ONCE_MIN           = 0;
    int         SPAWN_AMOUNT_ONCE_RANDOM_RANGE  = 0;
    int         SPAWN_AMOUNT_INITIAL            = 0;
    Vector3     SPAWN_RANGE                     = Vector3::Zero;
    Vector3     SPAWN_FIRST_VELOCITY            = Vector3::Zero;
    Vector3     SPAWNER_POSITION                = Vector3::Zero;
    std::string SPAWNER_NAME                    = "NO NAME SPAWNER.";
    
    // �X�|�[�������֐��|�C���^�Bnullptr�Ȃ疳�����ŃX�|�[���B
    bool(*SPAWN_CONDITION)(void)            = nullptr; 
};