#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/util/math/Vector3.h"
#include "src/scene/play/spawner/SpawnerDescs.h"

/// <summary>
/// �L�����̃X�|�i�[
/// </summary>
/// <author>���{��</author>
class CharaSpawnPoint : public Object3D
{
public:
    CharaSpawnPoint();
    ~CharaSpawnPoint();

    void Init(const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);
    void Draw() override;

    // �g�p�񐔂𑝂₷
    void Use() { m_UseCount++; }
    // �g�p�񐔂��擾
    int GetUseCount() const { return m_UseCount; }
private:
    Vector3     m_InitialVelocity;  // �������Ƃ��ɑΏۂɗ^�����鏉��
    int         m_UseCount;         // �g�p��
};