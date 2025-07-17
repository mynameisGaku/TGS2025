#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/util/math/Vector3.h"
#include "src/scene/play/spawner/SpawnerDescs.h"

/// <summary>
/// キャラのスポナー
/// </summary>
/// <author>藤本樂</author>
class CharaSpawnPoint : public Object3D
{
public:
    CharaSpawnPoint();
    ~CharaSpawnPoint();

    void Init(const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);
    void Draw() override;

    // 使用回数を増やす
    void Use() { m_UseCount++; }
    // 使用回数を取得
    int GetUseCount() const { return m_UseCount; }
private:
    Vector3     m_InitialVelocity;  // 沸いたときに対象に与えられる初速
    int         m_UseCount;         // 使用回数
};