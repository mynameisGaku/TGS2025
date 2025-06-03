#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/util/math/Vector3.h"
#include "src/scene/play/spawner/SpawnerDescs.h"

class CharaSpawnPoint : public Object3D
{
public:
    CharaSpawnPoint();
    ~CharaSpawnPoint();

    void Init(const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);
    void Draw() override;

    void Use() { m_UseCount++; }
    int GetUseCount() const { return m_UseCount; }
private:
    Vector3 m_InitialVelocity;
    int m_UseCount;
};