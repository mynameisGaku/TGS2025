#pragma once
#include <list>
#include <unordered_map>
#include "framework/gameObject.h"
#include "src/scene/play/chara/CharaSpawnPoint.h"

class CharaSpawnPointManager : public GameObject
{
public:
    CharaSpawnPointManager();
    ~CharaSpawnPointManager();

    void Update() override;
    void Draw() override;

    void AddSpawnPoint(int hModel, const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);

    CharaSpawnPoint* Get_Near(const Vector3& pos);
    CharaSpawnPoint* Get_Far(const Vector3& pos);
    CharaSpawnPoint* Get_Random();
    CharaSpawnPoint* Get_LowUsageRate();
    CharaSpawnPoint* Get_HighUsageRate();
private:
    std::list<CharaSpawnPoint*> m_SpawnPoints;
};