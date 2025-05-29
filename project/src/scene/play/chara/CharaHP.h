#pragma once
#include "src/util/time/GameTime.h"
#include "framework/gameObject.h"
#include <algorithm>
#include <functional>
#include <deque>
#include <string>

class CharaHP : public GameObject
{
public:
    CharaHP();
    ~CharaHP() = default;

    /// HPを回復する
    void Heal(float add);

    /// HPを回復する(デフォルト値使用)
    void Heal_UseDefault();

    /// ダメージを受ける
    void Damage(float sub);

    // ダメージを受ける(デフォルト値使用)
    void Damage_UseDefault();

    /// 最大値を設定する
    void SetMaxHP(float maxHP);

    /// 最大値を設定する(比率を保持する)
    void SetMaxHP_KeepRatio(float maxHP);

    /// 現在の値をセットする
    void SetCurrent(float value);

    /// 現在の値を取得する
    float GetCurrent() const;

    /// 最大値を取得する
    float GetMax() const;

    /// HP割合を取得する
    float GetRatio() const;

    /// 死亡しているかを取得する
    bool IsDead() const;

    /// 最小HPを設定（0以上）
    void SetMinHP(float value);

    /// オーバーヒールを許可・不許可設定
    void SetOverhealEnabled(bool enable);

    /// コールバック設定
    void SetOnDeadCallback(std::function<void()> callback);
    void SetOnHealFullCallback(std::function<void()> callback);

    struct HPChangeLog
    {
        float amount;
        float time;
        std::string source;
    };
    /// ログ取得
    const std::deque<HPChangeLog>& GetLog() const;

    /// HPリセット
    void Reset();

private:
    float m_DefaultDamage = 0.0f;
    float m_DefaultHeal = 0.0f;
    float m_Current = 0.0f;
    float m_Max = 0.0f;
    float m_MinHP = 0.0f;
    bool m_AllowOverheal = false;

    std::function<void()> m_OnDead;
    std::function<void()> m_OnHealFull;

    std::deque<HPChangeLog> m_Log;
    static constexpr size_t LOG_LIMIT = 50;

    void ClampCurrentHP();
    void LogChange(float amount, const std::string& source);
};