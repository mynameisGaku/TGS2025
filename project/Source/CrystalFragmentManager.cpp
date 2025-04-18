#include "CrystalFragmentManager.h"
#include "CrystalFragmentRef.h"
#include "CrystalFragment.h"

CrystalFragmentManager::CrystalFragmentManager()
{
    // リファレンスロード
    auto& ref = CRYSTALFRAGMENT_REF;
    ref.Load(); // 内部で重複して読み込まないようにしています。
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
            // 無効化されたら削除
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

    // 各種リファレンスを元に初期化
    fragment->transform->position   = pos;
    fragment->m_RotSpeed            = ref.m_RotSpeed;
    fragment->m_ElementPoint        = ref.m_ElementPoint;
    fragment->m_Size                = ref.m_Size;
    fragment->m_Lifetime            = ref.m_LifeTime;

    // 追加
    m_CrystalFragments.push_back(fragment);
}
