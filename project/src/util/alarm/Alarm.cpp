#include "src/util/alarm/Alarm.h"
#include "src/util/time/GameTime.h"

Alarm::Alarm()
{
    m_Timer = 0.0f;
}

Alarm::~Alarm()
{
}

void Alarm::Update()
{
    if (not IsFinish())
    {
        m_Timer -= GTime.deltaTime;
    }
}

bool Alarm::IsFinish() const
{
    return m_Timer <= 0.0f;
}

void Alarm::Set(float t)
{
    if (not IsFinish())
        return;

    m_Timer = t;
}

void Alarm::Reset()
{
    m_Timer = 0.0f;
}
