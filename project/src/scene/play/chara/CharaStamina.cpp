#include "src/scene/play/chara/CharaStamina.h"
#include "src/reference/chara/CharaStaminaRef.h"
#include <algorithm>
#include <cmath>

// min/max �}�N��������
#undef min
#undef max

CharaStamina::CharaStamina()
{
    // �X�^�~�i�̍ő�l���擾
    CHARASTAMINA_REF.Load();
    m_Max            = CHARASTAMINA_REF.Max;
    m_RegenTakesTime = CHARASTAMINA_REF.RegenTakesTime;
    m_RegenStartTime = CHARASTAMINA_REF.RegenStartTime;
    m_Current        = m_Max;
    m_Target         = m_Max;
    m_RegenTimeCount = 0.0f;
    m_RegenStartTimeCount = 0.0f;
    m_IsNeedRegen    = false;
}

void CharaStamina::Update()
{
    if (not m_IsNeedRegen)
        return;

    if(m_RegenStartTimeCount > 0.0f)
    {
        m_RegenStartTimeCount -= GTime.deltaTime;
        return;
    }

    // RegenTakesTime�b�����đS�񕜂���
    m_RegenTimeCount += GTime.deltaTime;
    m_Current = std::lerp(m_Current, m_Max, m_RegenTimeCount);

    if (m_Current > m_Max)
    {
        m_Current = m_Max;
        m_RegenTimeCount = 0.0f;
        m_IsNeedRegen = false;
    }
}

void CharaStamina::Use(float sub)
{
    // �f�t�H���g�����̏ꍇ�A1�b��1.0f����
    m_Current = std::max(0.0f, m_Current - sub);
    m_RegenStartTimeCount = m_RegenStartTime;
    m_IsNeedRegen = true;
}

void CharaStamina::SetMaxStamina(float maxStamina)
{
    m_Max = maxStamina;
    m_Current = std::clamp(m_Current, 0.0f, m_Max);
}

void CharaStamina::SetCurrent(float value)
{
    m_Current = std::clamp(value, 0.0f, m_Max);
}

float CharaStamina::GetCurrent() const
{
    return m_Current;
}

float CharaStamina::GetMax() const
{
    return m_Max;
}

void CharaStamina::Reset()
{
    m_Current = m_Max;
}
