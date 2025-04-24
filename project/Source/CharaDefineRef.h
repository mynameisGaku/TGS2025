#pragma once

#include <vector>
#include <string>

class CharaDefineRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaDefine.json";         // �t�@�C���̃p�X
    const std::string   FILEKEY  = "CharaDefineRef";                           // �t�@�C�����ʗp�L�[
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

    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

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