#include "src/util/alarm/Alarm.h"
#include "src/util/time/GameTime.h"

Alarm::Alarm()
{
    m_TimerSec = 0.0f;
    m_Finish = true;
}

Alarm::~Alarm()
{
}

void Alarm::Update()
{
    if (m_TimerSec > 0.0f)
    {
        m_TimerSec -= GTime.deltaTime;
        m_Finish = false;
    }
    else
    {
        m_TimerSec = 0.0f;
        m_Finish = true;
    }
}

bool Alarm::IsFinish() const
{
    return m_Finish;
}

void Alarm::Set(float t)
{
    if (not m_Finish)
        return;

    m_TimerSec = t;
}

void Alarm::Reset()
{
    m_TimerSec = 0.0f;
}
