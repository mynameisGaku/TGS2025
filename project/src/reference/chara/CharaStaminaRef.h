#pragma once
#include <string>

/// <summary>
/// キャラのスタミナに関する設定を保持するクラス
/// Jsonから読みこんだパラメーターを保持するよ
/// </summary>
class CharaStaminaRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaStamina.json";         // ファイルのパス
    const std::string   FILEKEY  = "CharaStaminaRef";                           // ファイル識別用キー

    static CharaStaminaRef* instance;
public:

    static CharaStaminaRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CharaStaminaRef;
        }
        return instance;
    }

    // " リファレンス解放 "
    // ↑を文字列検索したら着くところで解放してます。(おそらくメインループのすぐあと)
    void Destroy()
    {
        if (instance)
        {
            delete instance;
        }
        instance = nullptr;
    }

    float Max;              // 最大スタミナ
    float RegenTakesTime;   // 回復にかかる時間
    float RegenStartTime;   // 回復開始までの時間
    float RegenStartTime_AllLost;   // スタミナが切れた際の回復開始までの時間

    void Load(bool ForceLoad = false);

private:
    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CharaStaminaRef() :
        Max(0.0f),
        RegenTakesTime(0.0f),
        RegenStartTime(0.0f),
        RegenStartTime_AllLost(0.0f)
    { /*DO NOTHING*/ }

    ~CharaStaminaRef()
    { /*DO NOTHING*/ }

};

#define CHARASTAMINA_REF (*CharaStaminaRef::Inst())