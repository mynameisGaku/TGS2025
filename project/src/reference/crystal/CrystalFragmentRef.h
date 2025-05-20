#pragma once
#include <string>
#include <vector>

/// <summary>
/// CrystalFragment.json�̃p�����[�^�[��ێ�����N���X
/// Json����ǂ݂��񂾃p�����[�^�[��ێ������
/// </summary>
class CrystalFragmentRef
{
private:
    enum InfoType
    {
        NONE,
        SMALL,
        MEDIUM,
        LARGE,
        MAX
    };
private:
    const std::string   FILEPATH = "data/Json/Crystal/CrystalFragment.json";    // �t�@�C���̃p�X
    const std::string   FILEKEY  = "CrystalFragment";                           // �t�@�C�����ʗp�L�[
public:
    static CrystalFragmentRef& Inst()
    {
        static CrystalFragmentRef instance;
        return instance;
    }

    void Load(bool _ForceLoad = false);

    struct Info
    {
        float               RotSpeed;     // �j�Ђ�Y����]���x
        float               ElementPoint; // �����|�C���g
        float               Size;         // �傫��
        float               LifeTime;     // ����
    };

    std::vector<Info>   m_Refs;       // �e�T�C�Y�̃p�����[�^�[
    int                 m_LimitMax;   // ���������ʂ̌��x

private:
    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CrystalFragmentRef():
        m_LimitMax(0)
    { /*DO NOTHING*/ }

    ~CrystalFragmentRef()
    { /*DO NOTHING*/ }

};

#define CRYSTALFRAGMENT_REF CrystalFragmentRef::Inst()