#pragma once
#include "src/util/math/Vector3.h"
#include <string>

/// <summary>
/// スポナーの構成情報
/// </summary>


struct BALL_SPAWNER_DESC
{
    float       INTERVAL_SEC                    = 0.0f;
    float       INTERVAL_SEC_RANDOM_RANGE       = 0.0f;
    int         SPAWN_AMOUNT_ONCE_MAX           = 0;
    int         SPAWN_AMOUNT_ONCE_MIN           = 0;
    int         SPAWN_AMOUNT_ONCE_RANDOM_RANGE  = 0;
    int         SPAWN_AMOUNT_INITIAL            = 0;
    Vector3     SPAWN_RANGE                     = Vector3::Zero;
    Vector3     SPAWN_INITIAL_VELOCITY            = Vector3::Zero;
};

struct CHARA_SPAWN_POINT_DESC
{
    Vector3     SPAWN_INITIAL_VELOCITY            = Vector3::Zero;
};