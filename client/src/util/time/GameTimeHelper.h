#pragma once

/// <summary>
/// 指定秒数の経過を待つ簡易タイマー（Updateベース）
/// </summary>
class WaitForSeconds
{
public:
    explicit WaitForSeconds(float duration);
    void Update();
    bool IsFinished() const;

private:
    float m_Duration;
    float m_Elapsed;
};

/// <summary>
/// FixedUpdate単位でカウントする固定タイマー
/// </summary>
class FixedTimer
{
public:
    explicit FixedTimer(float duration);
    void Tick();
    bool IsFinished() const;
    void Reset();

private:
    float m_Duration;
    float m_Remaining;
};
