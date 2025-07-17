#include "StatusTracker.h"

void StatusTracker::AddKillCount(int add)
{
    m_KillCount += add;
}

void StatusTracker::AddDeathCount(int add)
{
    m_DeathCount += add;
}

void StatusTracker::AddHitCount(int add)
{
    m_HitCount += add;
}

void StatusTracker::AddGetHitCount(int add)
{
    m_GetHitCount += add;
}

void StatusTracker::AddThrowCount(int add)
{
    m_ThrowCount += add;
}

void StatusTracker::AddCatchCount(int add)
{
    m_CatchCount += add;
}

int StatusTracker::Get_KillCount() const
{
    return m_KillCount;
}

int StatusTracker::Get_DeathCount() const
{
    return m_DeathCount;
}

int StatusTracker::Get_HitCount() const
{
    return m_HitCount;
}

int StatusTracker::Get_GetHitCount() const
{
    return m_GetHitCount;
}

int StatusTracker::Get_ThrowCount() const
{
    return m_ThrowCount;
}

int StatusTracker::Get_CatchCount() const
{
    return m_CatchCount;
}

float StatusTracker::GetKD() const
{
    float kd = 0.0f;

    kd = (float)m_KillCount / (float)m_DeathCount;

    return kd;
}

float StatusTracker::GetAccuracy() const
{
    float acc = 0.0f;

    acc = (float)m_HitCount / (float)m_ThrowCount;

    return acc;
}
