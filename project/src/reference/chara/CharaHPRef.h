#pragma once
#include <string>

/// <summary>
/// キャラのHPに関する設定を保持するクラス
/// Jsonから読みこんだパラメーターを保持するよ
/// </summary>
class CharaHPRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaHP.json";         // ファイルのパス
    const std::string   FILEKEY = "CharaHPRef";                           // ファイル識別用キー
public:

    static CharaHPRef& Inst()
    {
        static CharaHPRef instance;
        return instance;
    }

    float Max;              // 最大HP
    float Damage;           // ダメージ量
    float Heal;             // 回復量

    void Load(bool ForceLoad = false);

private:
    bool m_WasLoad = false; // ロード済みか？ 重複回避用

    CharaHPRef() :
        Max(0.0f),
        Damage(0.0f),
        Heal(0.0f)
    { /*DO NOTHING*/ }

    ~CharaHPRef()
    { /*DO NOTHING*/ }

};

#define CHARAHP_REF CharaHPRef::Inst()