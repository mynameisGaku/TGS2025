#pragma once
#include <framework/gameObject.h>

class Alarm : public GameObject
{
public:
    Alarm();
    ~Alarm();

    void Update() override;

    bool IsFinish() const;

    // 秒数をセット　前回セットされていた場合、上書きされない
    void Set(float sec);

    void Reset();

private:
    float m_Timer;
};