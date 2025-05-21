#pragma once
#include "src/util/time/GameTime.h"
#include "framework/gameObject.h"
#include <utility>

/// <summary>
/// スタミナクラス
/// </summary>
class CharaStamina : public GameObject
{
public:
    CharaStamina();
    ~CharaStamina()
    { /*DO NOTHING*/ }

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

    /// <summary>
    /// スタミナを使用(消費)する
    /// </summary>
    /// <param name="speed">減る速度(秒)</param>
    void Use(float sub);

    /// <summary>
    /// 最大値を設定する
    /// </summary>
    /// <param name="maxStamina">最大値</param>
    void SetMaxStamina(float maxStamina);

    /// <summary>
    /// 現在の値をセットする
    /// </summary>
    /// <param name="current">設定したい値</param>
    void SetCurrent(float value);

    /// <summary>
    /// 現在の値を取得する
    /// </summary>
    float GetCurrent() const;

    /// <summary>
    /// 最大値を取得する
    /// </summary>
    float GetMax() const;

private:
    float   m_Target;               // 目標値 現在未使用
    float   m_Current;              // 現在の値
    float   m_Max;                  // 最大値
    float   m_RegenTakesTime;       // 回復にかかる時間
    float   m_RegenTimeCount;       // 回復中のカウント
    float   m_RegenStartTime;       // 回復開始までの時間
    float   m_RegenStartTimeCount;  // 回復開始までのカウント
    bool    m_IsNeedRegen;          // 回復が必要か？
};