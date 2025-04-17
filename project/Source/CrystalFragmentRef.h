#pragma once
#include <string>

/// <summary>
/// CrystalFragment.json�̃p�����[�^�[��ێ�����N���X
/// Json����ǂ݂��񂾃p�����[�^�[��ێ������
/// </summary>
class CrystalFragmentRef
{
private:
    const std::string   FILENAME = "data/Json/Crystal/CrystalFragment.json";
    const std::string   FILEKEY  = "CrystalFragment";
public:
    static CrystalFragmentRef& Inst()
    {
        static CrystalFragmentRef instance;
        return instance;
    }

    void Load(bool _ForceLoad = false);

    float               m_RotSpeed;
    float               m_ElementPoint;
    float               m_Size;
    float               m_LifeTime;

private:
    bool                m_WasLoad = false;

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