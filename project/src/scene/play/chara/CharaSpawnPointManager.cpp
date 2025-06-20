#include "CharaSpawnPointManager.h"

#include <limits>
#include "src/util/math/Random.h"

#undef max
#undef min

CharaSpawnPointManager::CharaSpawnPointManager()
{
}

CharaSpawnPointManager::~CharaSpawnPointManager()
{
}

void CharaSpawnPointManager::Update()
{
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        csp->Update();
    }

    for (auto it = m_SpawnPoints.begin(); it != m_SpawnPoints.end();)
    {
        auto csp = (*it);
        if (csp->IsDestroy())
        {
            it = m_SpawnPoints.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CharaSpawnPointManager::Draw()
{
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        csp->Draw();
    }
}

void CharaSpawnPointManager::AddSpawnPoint(int hModel, const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc)
{
    // csp = chara spawn point
    CharaSpawnPoint* csp = Instantiate<CharaSpawnPoint>();
    csp->Init(trs, desc);
    csp->SetModel(hModel);
    m_SpawnPoints.push_back(csp);
}

CharaSpawnPoint* CharaSpawnPointManager::Get_Near(const Vector3& pos)
{
    CharaSpawnPoint* result = nullptr;
    float len = std::numeric_limits<float>::max();
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        Vector3 distance = csp->transform->position - pos;
        float dlen = distance.GetLength();
        if (dlen < len)
        {
            len = dlen;
            result = csp;
        }
    }
    return result;
}

CharaSpawnPoint* CharaSpawnPointManager::Get_Far(const Vector3& pos)
{
    CharaSpawnPoint* result = nullptr;
    float len = 0;
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        Vector3 distance = csp->transform->position - pos;
        float dlen = distance.GetLength();
        if (dlen > len)
        {
            len = dlen;
            result = csp;
        }
    }
    return result;
}

CharaSpawnPoint* CharaSpawnPointManager::Get_Random()
{
    int r = Random.GetIntRange(0, m_SpawnPoints.size() - 1);
    int c = 0;
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        if (c == r)
        {
            return csp;
        }
        c++;
    }
    return nullptr;
}

CharaSpawnPoint* CharaSpawnPointManager::Get_LowUsageRate()
{
    CharaSpawnPoint* result = nullptr;
    int low = std::numeric_limits<int>::max();
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        if (csp->GetUseCount() < low)
        {
            low = csp->GetUseCount();
            result = csp;
        }
    }
    return result;
}

CharaSpawnPoint* CharaSpawnPointManager::Get_HighUsageRate()
{
    CharaSpawnPoint* result = nullptr;
    int high = 0;
    // csp = chara spawn point
    for (auto& csp : m_SpawnPoints)
    {
        if (csp->GetUseCount() > high)
        {
            high = csp->GetUseCount();
            result = csp;
        }
    }
    return result;
}