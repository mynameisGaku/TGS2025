#include "GameTimeHelper.h"
#include "GameTime.h"

WaitForSeconds::WaitForSeconds(float duration)
    : m_Duration(duration), m_Elapsed(0.0f)
{
}

void WaitForSeconds::Update()
{
    // スケール適用（HitStop中は0）
    m_Elapsed += GameTime::DeltaTime();
}

bool WaitForSeconds::IsFinished() const
{
    return m_Elapsed >= m_Duration;
}

FixedTimer::FixedTimer(float duration)
    : m_Duration(duration), m_Remaining(duration)
{
}

void FixedTimer::Tick()
{
    // Fixedは「一定Δt」を刻む（スケール非依存）
    m_Remaining -= GameTime::FixedUnscaledDeltaTime();
}

bool FixedTimer::IsFinished() const
{
    return m_Remaining <= 0.0f;
}

void FixedTimer::Reset()
{
    m_Remaining = m_Duration;
}
