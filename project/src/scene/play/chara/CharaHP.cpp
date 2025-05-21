#include "src/scene/play/chara/CharaHP.h"
#include "src/reference/chara/CharaHPRef.h"
#include "src/util/time/GameTime.h"
#include <Windows.h>

CharaHP::CharaHP()
{
    CHARAHP_REF.Load();
    m_Max           = CHARAHP_REF.Max;
    m_DefaultDamage = CHARAHP_REF.Damage;
    m_DefaultHeal   = CHARAHP_REF.Heal;
    m_Current       = m_Max;
    m_MinHP         = 0.0f;
    m_Log.resize(LOG_LIMIT);
}

void CharaHP::Heal(float add)
{
    if (add <= 0.0f) return;
    m_Current += add;
    LogChange(add, "Heal");

    if (!m_AllowOverheal && m_Current > m_Max)
    {
        m_Current = m_Max;
        if (m_OnHealFull) m_OnHealFull();
    }
}

void CharaHP::Heal_UseDefault()
{
    Heal(m_DefaultHeal);
}

void CharaHP::Damage(float sub)
{
    if (sub <= 0.0f) return;
    m_Current -= sub;
    LogChange(-sub, "Damage");

    if (m_Current < m_MinHP)
    {
        m_Current = m_MinHP;
    }
    if (IsDead() && m_OnDead)
    {
        m_OnDead();
    }
}

void CharaHP::Damage_UseDefault()
{
    Damage(m_DefaultDamage);
}

void CharaHP::SetMaxHP(float maxHP)
{
    m_Max = max(0.0f, maxHP);
    ClampCurrentHP();
}

void CharaHP::SetMaxHP_KeepRatio(float maxHP)
{
    float ratio = (m_Max > 0.0f) ? (m_Current / m_Max) : 0.0f;
    m_Max       = max(0.0f, maxHP);
    m_Current   = ratio * m_Max;
    ClampCurrentHP();
}

void CharaHP::SetCurrent(float value)
{
    float old = m_Current;
    m_Current = value;
    ClampCurrentHP();
    LogChange(m_Current - old, "SetCurrent");
}

float CharaHP::GetCurrent() const
{
    return m_Current;
}

float CharaHP::GetMax() const
{
    return m_Max;
}

float CharaHP::GetRatio() const
{
    return (m_Max > 0.0f) ? (m_Current / m_Max) : 0.0f;
}

bool CharaHP::IsDead() const
{
    return m_Current <= m_MinHP;
}

void CharaHP::SetMinHP(float value)
{
    m_MinHP = std::clamp(value, 0.0f, m_Max);
    ClampCurrentHP();
}

void CharaHP::SetOverhealEnabled(bool enable)
{
    m_AllowOverheal = enable;
}

void CharaHP::SetOnDeadCallback(std::function<void()> callback)
{
    m_OnDead = std::move(callback);
}

void CharaHP::SetOnHealFullCallback(std::function<void()> callback)
{
    m_OnHealFull = std::move(callback);
}

void CharaHP::ClampCurrentHP()
{
    if (!m_AllowOverheal && m_Current > m_Max)
        m_Current = m_Max;
    if (m_Current < m_MinHP)
        m_Current = m_MinHP;
}

void CharaHP::LogChange(float amount, const std::string& source)
{
    if (m_Log.size() >= LOG_LIMIT)
    {
        m_Log.pop_front();
    }
    HPChangeLog log;
    log.amount = amount;
    log.time = GTime.TotalTime();
    log.source = source;
    m_Log.push_back(log);
}

const std::deque<CharaHP::HPChangeLog>& CharaHP::GetLog() const
{
    return m_Log;
}

void CharaHP::ResetHP()
{
    m_Current = m_Max;
    m_Log.clear();
    LogChange(m_Current, "Reset");
}
