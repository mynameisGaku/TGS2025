#pragma once
#include <framework/gameObject.h>

/// <summary>
/// 目覚まし時計のアラーム的な機能を持つクラス
/// </summary>
/// <author>藤本樂</author>
class Alarm : public GameObject
{
public:
    Alarm();
    ~Alarm();

    void Update() override;

    // アラームが鳴っているか
    bool IsFinish() const;

    // 秒数をセット　前回セットされていた場合、上書きされない
    void Set(float sec);

    // アラームをリセットする。２度目以降のSetの前に呼ぶ必要があります。
    void Reset();

private:
    float   m_TimerSec;   // 時間計測(秒)
    bool    m_Finish;     // アラームが鳴っているか？
};