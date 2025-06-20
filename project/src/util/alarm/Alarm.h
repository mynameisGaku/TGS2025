#pragma once
#include <framework/gameObject.h>

/// <summary>
/// �ڊo�܂����v�̃A���[���I�ȋ@�\�����N���X
/// </summary>
/// <author>���{��</author>
class Alarm : public GameObject
{
public:
    Alarm();
    ~Alarm();

    void Update() override;

    // �A���[�������Ă��邩
    bool IsFinish() const;

    // �b�����Z�b�g�@�O��Z�b�g����Ă����ꍇ�A�㏑������Ȃ�
    void Set(float sec);

    // �A���[�������Z�b�g����B�Q�x�ڈȍ~��Set�̑O�ɌĂԕK�v������܂��B
    void Reset();

private:
    float   m_TimerSec;   // ���Ԍv��(�b)
    bool    m_Finish;     // �A���[�������Ă��邩�H
};