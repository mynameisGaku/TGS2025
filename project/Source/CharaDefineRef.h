#pragma once

#include <vector>
#include <string>

class CharaDefineRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaDefine.json";         // ファイルのパス
    const std::string   FILEKEY  = "CharaDefineRef";                           // ファイル識別用キー
public:
    static CharaDefineRef& Inst()
    {
        static CharaDefineRef instance;
        return instance;
    }

    int                         Max;
    std::vector<std::string>    Tags;
    float                       CatchRadius;
    
    void Load(bool ForceLoad = false);
private:

    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CharaDefineRef() :
        Max(0),
        Tags({}),
        CatchRadius(0.0f)
    { /*DO NOTHING*/
    }

    ~CharaDefineRef()
    { /*DO NOTHING*/
    }
};

#define CHARADEFINE_REF CharaDefineRef::Inst()