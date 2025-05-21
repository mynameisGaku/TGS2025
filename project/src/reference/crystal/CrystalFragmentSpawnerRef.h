#pragma once
#include <string>

class CrystalFragmentSpawnerRef
{
private:
    const std::string   FILEPATH = "data/Json/Crystal/CrystalFragmentSpawner.json"; // �t�@�C���̃p�X
    const std::string   FILEKEY = "CrystalFragmentSpawner";                         // �t�@�C�����ʗp�L�[

    static CrystalFragmentSpawnerRef* instance;
public:
    static CrystalFragmentSpawnerRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CrystalFragmentSpawnerRef;
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

    void Load(bool ForceLoad = false);

    float               SpawnInterval;  // �����܂ł̃C���^�[�o��
    float               SpawnAmount;    // ��x�Ő������鐔

private:
    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CrystalFragmentSpawnerRef() :
        SpawnInterval(0.0f),
        SpawnAmount(0.0f)
    { /*DO NOTHING*/ };
    ~CrystalFragmentSpawnerRef()
    { /*DO NOTHING*/ };
};

#define CRYSTALFRAGMENTSPAWNER_REF (*CrystalFragmentSpawnerRef::Inst())