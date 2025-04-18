#pragma once
#include <string>

/// <summary>
/// CrystalFragment.json�̃p�����[�^�[��ێ�����N���X
/// Json����ǂ݂��񂾃p�����[�^�[��ێ������
/// </summary>
class CrystalFragmentRef
{
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

    float               m_RotSpeed;     // �j�Ђ�Y����]���x
    float               m_ElementPoint; // �����|�C���g
    float               m_Size;         // �傫��
    float               m_LifeTime;     // ����

private:
    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CrystalFragmentRef() :
        m_RotSpeed    (0.0f),
        m_ElementPoint(0.0f),
        m_Size        (0.0f),
        m_LifeTime    (0.0f)
    { /*DO NOTHING*/ }

    ~CrystalFragmentRef()
    { /*DO NOTHING*/ }

};

#define CRYSTALFRAGMENT_REF CrystalFragmentRef::Inst()