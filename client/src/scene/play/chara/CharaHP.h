#pragma once
#include "src/util/time/GameTime.h"
#include "framework/gameObject.h"
#include <algorithm>
#include <functional>
#include <deque>
#include <string>

class CharaHP : public GameObject
{
public:
    CharaHP();
    ~CharaHP() = default;

    /// HP���񕜂���
    void Heal(float add);

    /// HP���񕜂���(�f�t�H���g�l�g�p)
    void Heal_UseDefault();

    /// �_���[�W���󂯂�
    void Damage(float sub);

    // �_���[�W���󂯂�(�f�t�H���g�l�g�p)
    void Damage_UseDefault();

    /// �ő�l��ݒ肷��
    void SetMaxHP(float maxHP);

    /// �ő�l��ݒ肷��(�䗦��ێ�����)
    void SetMaxHP_KeepRatio(float maxHP);

    /// ���݂̒l���Z�b�g����
    void SetCurrent(float value);

    /// ���݂̒l���擾����
    float GetCurrent() const;

    /// �ő�l���擾����
    float GetMax() const;

    /// HP�������擾����
    float GetRatio() const;

    /// ���S���Ă��邩���擾����
    bool IsDead() const;

    /// �ŏ�HP��ݒ�i0�ȏ�j
    void SetMinHP(float value);

    /// �I�[�o�[�q�[�������E�s���ݒ�
    void SetOverhealEnabled(bool enable);

    /// �R�[���o�b�N�ݒ�
    void SetOnDeadCallback(std::function<void()> callback);
    void SetOnHealFullCallback(std::function<void()> callback);

    struct HPChangeLog
    {
        float amount;
        float time;
        std::string source;
    };
    /// ���O�擾
    const std::deque<HPChangeLog>& GetLog() const;

    /// HP���Z�b�g
    void Reset();

private:
    float m_DefaultDamage = 0.0f;
    float m_DefaultHeal = 0.0f;
    float m_Current = 0.0f;
    float m_Max = 0.0f;
    float m_MinHP = 0.0f;
    bool m_AllowOverheal = false;

    std::function<void()> m_OnDead;
    std::function<void()> m_OnHealFull;

    std::deque<HPChangeLog> m_Log;
    static constexpr size_t LOG_LIMIT = 50;

    void ClampCurrentHP();
    void LogChange(float amount, const std::string& source);
};