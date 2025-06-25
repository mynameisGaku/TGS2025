#pragma once

#include <vector>
#include <string>

class CharaDefineRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaDefine.json";         // ファイルのパス
    const std::string   FILEKEY  = "CharaDefineRef";                           // ファイル識別用キー

    static CharaDefineRef* instance;
public:
    static CharaDefineRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CharaDefineRef;
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

    int                         Max{};
    std::vector<std::string>    Tags{};
    float                       CatchRadius{};
    float                       EmoteInterval{};
    float                       MoveSpeed{};
    float                       RotSpeed{};
    float                       JumpPower{};
    float                       TackleRadius{};
    float                       TackleInterval{};         // タックルできるまでの間隔(秒)
    float                       TackleRecovery{};         // 後隙(秒)
    float                       TackleForce_Vertical{};
    float                       TackleForce_Horizontal{};
    float                       GetTackleInvincibleTime{};  // タックルを食らったときに付与される無敵時間(秒)
    
    void Load(bool ForceLoad = false);
private:

    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CharaDefineRef()
    { /*DO NOTHING*/
    }

    ~CharaDefineRef()
    { /*DO NOTHING*/
    }
};

#define CHARADEFINE_REF (*CharaDefineRef::Inst())