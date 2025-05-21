#include "GameTimeHelper.h"
#include "GameTime.h"

WaitForSeconds::WaitForSeconds(float duration)
    : m_Duration(duration), m_Elapsed(0.0f)
{}

void WaitForSeconds::Update()
{
    // �t���[�����ԕ������i�߂�
    m_Elapsed += GTime.deltaTime;
}

bool WaitForSeconds::IsFinished() const
{
    // �w�莞�Ԃ��z�����犮��
    return m_Elapsed >= m_Duration;
}

FixedTimer::FixedTimer(float duration)
    : m_Duration(duration), m_Remaining(duration)
{}

void FixedTimer::Tick()
{
    // FixedDeltaTime���ƂɃJ�E���g�_�E��
    m_Remaining -= GameTime::FixedDeltaTime();
}

bool FixedTimer::IsFinished() const
{
    // �c�肪0�ȉ��Ȃ�I��
    return m_Remaining <= 0.0f;
}

void FixedTimer::Reset()
{
    // ������Ԃɖ߂�
    m_Remaining = m_Duration;
}