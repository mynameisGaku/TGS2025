#include "GameTimeHelper.h"
#include "GameTime.h"

WaitForSeconds::WaitForSeconds(float duration)
    : m_Duration(duration), m_Elapsed(0.0f)
{}

void WaitForSeconds::Update()
{
    // フレーム時間分だけ進める
    m_Elapsed += GTime.deltaTime;
}

bool WaitForSeconds::IsFinished() const
{
    // 指定時間を越えたら完了
    return m_Elapsed >= m_Duration;
}

FixedTimer::FixedTimer(float duration)
    : m_Duration(duration), m_Remaining(duration)
{}

void FixedTimer::Tick()
{
    // FixedDeltaTimeごとにカウントダウン
    m_Remaining -= GameTime::FixedDeltaTime();
}

bool FixedTimer::IsFinished() const
{
    // 残りが0以下なら終了
    return m_Remaining <= 0.0f;
}

void FixedTimer::Reset()
{
    // 初期状態に戻す
    m_Remaining = m_Duration;
}