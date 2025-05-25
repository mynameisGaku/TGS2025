#pragma once

/// <summary>
/// ステータスの統計を取るためのトラッカー
/// キャラ以外で使える用
/// </summary>
class StatusTracker
{
public:

    StatusTracker()
        : m_KillCount(0)
        , m_DeathCount(0)
        , m_HitCount(0)
        , m_GetHitCount(0)
        , m_ThrowCount(0)
        , m_CatchCount(0) {}

    void AddKillCount(int add);
    void AddDeathCount(int add);
    void AddHitCount(int add);
    void AddGetHitCount(int add);
    void AddThrowCount(int add);
    void AddCatchCount(int add);

    int Get_KillCount() const;
    int Get_DeathCount() const;
    int Get_HitCount() const;
    int Get_GetHitCount() const;
    int Get_ThrowCount() const;
    int Get_CatchCount() const;

    float GetKD() const;
    float GetAccuracy() const;

private:
    int m_KillCount;
    int m_DeathCount;
    int m_HitCount;
    int m_GetHitCount;
    int m_ThrowCount;
    int m_CatchCount;
};