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
public:

    static CharaStaminaRef& Inst()
    {
        static CharaStaminaRef instance;
        return instance;
    }

    float Max;              // 最大スタミナ
    float RegenTakesTime;   // 回復にかかる時間
    float RegenStartTime;   // 回復開始までの時間

    void Load(bool ForceLoad = false);

private:
    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CharaStaminaRef() :
        Max(0.0f),
        RegenTakesTime(0.0f)
    { /*DO NOTHING*/ }

    ~CharaStaminaRef()
    { /*DO NOTHING*/ }

};

#define CHARASTAMINA_REF CharaStaminaRef::Inst()