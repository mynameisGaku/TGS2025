#include "CrystalFragmentManager.h"
#include "CrystalFragmentRef.h"

CrystalFragmentManager::CrystalFragmentManager()
{
    auto& ref = CRYSTALFRAGMENT_REF;
    ref.Load(); // “à•”‚Åd•¡‚µ‚Ä“Ç‚Ýž‚Ü‚È‚¢‚æ‚¤‚É‚µ‚Ä‚¢‚Ü‚·B
}

CrystalFragmentManager::~CrystalFragmentManager()
{
}

void CrystalFragmentManager::Update()
{
    for (auto it = m_CrystalFragments.begin(); it != m_CrystalFragments.end(); )
    {
        auto fragment = (*it);
        if (fragment->m_Lifetime <= 0.0f)
        {
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

    fragment->transform->position   = pos;
    fragment->m_RotSpeed            = ref.m_RotSpeed;
    fragment->m_ElementPoint        = ref.m_ElementPoint;
    fragment->m_Size                = ref.m_Size;
    fragment->m_Lifetime            = ref.m_LifeTime;

    m_CrystalFragments.push_back(fragment);
}
