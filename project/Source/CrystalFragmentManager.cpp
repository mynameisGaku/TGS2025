#include "CrystalFragmentManager.h"
#include "CrystalFragmentRef.h"
#include "CrystalFragment.h"

CrystalFragmentManager::CrystalFragmentManager()
{
    // ���t�@�����X���[�h
    auto& ref = CRYSTALFRAGMENT_REF;
    ref.Load(); // �����ŏd�����ēǂݍ��܂Ȃ��悤�ɂ��Ă��܂��B
}

CrystalFragmentManager::~CrystalFragmentManager()
{
}

void CrystalFragmentManager::Update()
{
    for (auto it = m_CrystalFragments.begin(); it != m_CrystalFragments.end(); )
    {
        auto fragment = (*it);
        if (not fragment->IsActive())
        {
            // ���������ꂽ��폜
            fragment->DestroyMe();
            fragment = nullptr;
            it = m_CrystalFragments.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CrystalFragmentManager::CreateFragment(const Vector3& pos)
{
    auto& ref = CRYSTALFRAGMENT_REF;
    auto fragment = Instantiate<CrystalFragment>();

    // �e�탊�t�@�����X�����ɏ�����
    fragment->transform->position   = pos;
    fragment->m_RotSpeed            = ref.m_RotSpeed;
    fragment->m_ElementPoint        = ref.m_ElementPoint;
    fragment->m_Size                = ref.m_Size;
    fragment->m_Lifetime            = ref.m_LifeTime;

    // �ǉ�
    m_CrystalFragments.push_back(fragment);
}
