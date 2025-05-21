#pragma once
#include "src/util/time/GameTime.h"
#include "framework/gameObject.h"
#include <utility>

/// <summary>
/// �X�^�~�i�N���X
/// </summary>
class CharaStamina : public GameObject
{
public:
    CharaStamina();
    ~CharaStamina()
    { /*DO NOTHING*/ }

    /// <summary>
    /// �X�V
    /// </summary>
    void Update() override;

    /// <summary>
    /// �X�^�~�i���g�p(����)����
    /// </summary>
    /// <param name="speed">���鑬�x(�b)</param>
    void Use(float sub);

    /// <summary>
    /// �ő�l��ݒ肷��
    /// </summary>
    /// <param name="maxStamina">�ő�l</param>
    void SetMaxStamina(float maxStamina);

    /// <summary>
    /// ���݂̒l���Z�b�g����
    /// </summary>
    /// <param name="current">�ݒ肵�����l</param>
    void SetCurrent(float value);

    /// <summary>
    /// ���݂̒l���擾����
    /// </summary>
    float GetCurrent() const;

    /// <summary>
    /// �ő�l���擾����
    /// </summary>
    float GetMax() const;

private:
    float   m_Target;               // �ڕW�l ���ݖ��g�p
    float   m_Current;              // ���݂̒l
    float   m_Max;                  // �ő�l
    float   m_RegenTakesTime;       // �񕜂ɂ����鎞��
    float   m_RegenTimeCount;       // �񕜒��̃J�E���g
    float   m_RegenStartTime;       // �񕜊J�n�܂ł̎���
    float   m_RegenStartTimeCount;  // �񕜊J�n�܂ł̃J�E���g
    bool    m_IsNeedRegen;          // �񕜂��K�v���H
};