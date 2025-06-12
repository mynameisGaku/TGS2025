#pragma once
#include <framework/gameObject.h>

class Alarm : public GameObject
{
public:
    Alarm();
    ~Alarm();

    void Update() override;

    bool IsFinish() const;

    // �b�����Z�b�g�@�O��Z�b�g����Ă����ꍇ�A�㏑������Ȃ�
    void Set(float sec);

    void Reset();

private:
    float m_Timer;
};