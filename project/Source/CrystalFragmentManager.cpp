#include "CrystalFragmentManager.h"
#include "CrystalFragmentRef.h"
#include "CrystalFragment.h"

#include "Pool.h" // Ensure Pool.h is explicitly included

CrystalFragmentManager::CrystalFragmentManager()
{
   // リファレンスロード
   auto& ref = CRYSTALFRAGMENT_REF;
   ref.Load(); // 内部で重複して読み込まないようにしています。

#ifdef USE_POOL
   m_pPool = new Pool<CrystalFragment>(ref.m_LimitMax);
#endif
}

CrystalFragmentManager::~CrystalFragmentManager()
{
#ifdef USE_POOL
    for (auto& item : m_pPool->GetAllItems())
    {
        if (item == nullptr)
            continue;

        if (item->m_pObject != nullptr)
        {
            item->m_pObject->DestroyMe();
            item->m_pObject = nullptr;
        }

        delete item;
        item = nullptr;
    }
    delete m_pPool;
#else

#endif
}

void CrystalFragmentManager::Update()
{
#ifdef USE_POOL
    for (auto& item : m_pPool->GetAllItems())
    {
        auto fragment = item->m_pObject;

        if (not fragment)
            continue;

        if (not fragment->IsActive())
        {
            m_pPool->DeActive(fragment->m_Index);
        }
    }
#else
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
#endif
}

void CrystalFragmentManager::Draw()
{
    m_pPool->PoolImGuiRendererBegin("crystal pool debug");

    ImGui::Text(("Capacity: " + std::to_string(m_pPool->GetCapacity())).c_str());
    ImGui::Text(("Active Items: " + std::to_string(m_pPool->GetActiveItemNum())).c_str());
    ImGui::Spacing();
    ImGui::Text("====================================");
    ImGui::Spacing();

    for (auto& item : m_pPool->GetAllItems())
    {
        if (item->m_pObject == nullptr)
            continue;

        ImGui::Text(("Lifetime :" + std::to_string(item->m_pObject->m_Lifetime)).c_str());
    }

    m_pPool->PoolImguiRendererEnd();
}

void CrystalFragmentManager::CreateFragment(const Vector3& pos)
{
    auto& ref = CRYSTALFRAGMENT_REF;
    // 各種リファレンスを元に初期化

    // 追加
#ifdef USE_POOL
    if (m_pPool->CheckActiveObjectByCount(m_pPool->GetCapacity()))
    {
        return;
    }
#endif


#ifdef USE_POOL
    uint32_t index = m_pPool->GetIndex();

    auto obj = m_pPool->Alloc([&](uint32_t i, CrystalFragment* p) { return initfunc(i, p); });

    obj->transform->position = pos;
    obj->m_RotSpeed = ref.m_RotSpeed;
    obj->m_ElementPoint = ref.m_ElementPoint;
    obj->m_Size = ref.m_Size;
    obj->m_Lifetime = ref.m_LifeTime;
    obj->SetIsActive(true);
    obj->SetRandFirstVelocity();

    m_pPool->SetObjectPointer(index, obj);

#else
    m_CrystalFragments.push_back(fragment);
#endif
}

CrystalFragment* CrystalFragmentManager::initfunc(uint32_t index, CrystalFragment* pFrag)
{
    pFrag = Instantiate<CrystalFragment>();
    pFrag->m_Index = index;
    return pFrag;
}
