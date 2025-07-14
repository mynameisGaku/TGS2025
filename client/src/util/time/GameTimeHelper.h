#pragma once

/// <summary>
/// �w��b���̌o�߂�҂ȈՃ^�C�}�[�iUpdate�x�[�X�j
/// </summary>
class WaitForSeconds
{
public:
    explicit WaitForSeconds(float duration);
    void Update();
    bool IsFinished() const;

private:
    float m_Duration;
    float m_Elapsed;
};

/// <summary>
/// FixedUpdate�P�ʂŃJ�E���g����Œ�^�C�}�[
/// </summary>
class FixedTimer
{
public:
    explicit FixedTimer(float duration);
    void Tick();
    bool IsFinished() const;
    void Reset();

private:
    float m_Duration;
    float m_Remaining;
};
