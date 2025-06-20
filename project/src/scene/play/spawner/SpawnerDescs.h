#pragma once
#include "src/util/math/Vector3.h"
#include <string>

// 各種スポナーの設定情報

/// <summary>
/// ボールスポナーの構成情報
/// </summary>
/// <author>藤本樂</author>
struct BALL_SPAWNER_DESC
{
    float       INTERVAL_SEC                    = 0.0f;             // スポーン間隔(秒)
    float       INTERVAL_SEC_RANDOM_RANGE       = 0.0f;             // スポーン間隔のランダム範囲(秒)
    int         SPAWN_AMOUNT_ONCE_MAX           = 0;                // 一度にスポーンする最大数
    int         SPAWN_AMOUNT_ONCE_MIN           = 0;                // 一度にスポーンする最小数
    int         SPAWN_AMOUNT_ONCE_RANDOM_RANGE  = 0;                // 一度にスポーンする数のランダム範囲
    int         SPAWN_AMOUNT_INITIAL            = 0;                // 初期スポーン数
    Vector3     SPAWN_RANGE                     = Vector3::Zero;    // スポーン位置の範囲
    Vector3     SPAWN_INITIAL_VELOCITY          = Vector3::Zero;    // スポーン時の初期速度
};

/// <summary>
/// キャラスポナーの構成情報
/// </summary>
/// <author>藤本樂</author>
struct CHARA_SPAWN_POINT_DESC
{
    Vector3     SPAWN_INITIAL_VELOCITY          = Vector3::Zero;    // スポーン時の初期速度
};