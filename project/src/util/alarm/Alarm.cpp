#include "src/util/alarm/Alarm.h"
#include "src/util/time/GameTime.h"

Alarm::Alarm()
{
    m_Timer = 0.0f;
    m_Finish = true;
}

Alarm::~Alarm()
{
}

void Alarm::Update()
{
    if (not m_Finish)
    {
        m_Timer -= GTime.deltaTime;
        if (m_Timer < 0.0f)
        {
            m_Finish = true;
        }
    }
    else
    {
        m_Timer = 0.0f;
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

    m_Timer = t;
}

void Alarm::Reset()
{
    m_Timer = 0.0f;
}
