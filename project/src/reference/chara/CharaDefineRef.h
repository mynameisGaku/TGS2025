#pragma once

#include <vector>
#include <string>

class CharaDefineRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaDefine.json";         // �t�@�C���̃p�X
    const std::string   FILEKEY  = "CharaDefineRef";                           // �t�@�C�����ʗp�L�[

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

    // " ���t�@�����X��� "
    // ���𕶎��񌟍������璅���Ƃ���ŉ�����Ă܂��B(�����炭���C�����[�v�̂�������)
    void Destroy()
    {
        if (instance)
        {
            delete instance;
        }
        instance = nullptr;
    }

    int                         Max;
    std::vector<std::string>    Tags;
    float                       CatchRadius;
    float                       EmoteInterval;
    float                       JumpPower;
    
    void Load(bool ForceLoad = false);
private:

    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CharaDefineRef() :
        Max(0),
        Tags({}),
        CatchRadius(0.0f),
        EmoteInterval(0.0f),
        JumpPower(0.0f)
    { /*DO NOTHING*/
    }

    ~CharaDefineRef()
    { /*DO NOTHING*/
    }
};

#define CHARADEFINE_REF (*CharaDefineRef::Inst())