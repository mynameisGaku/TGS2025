#pragma once
#include "object3d.h"
#include "Component/Physics.h"

/// <summary>
/// �N���X�^�����痎����j�ЃN���X
/// </summary>
class CrystalFragment : public Object3D
{
public:
    CrystalFragment();
    ~CrystalFragment();

    void Update() override;
    void Draw() override;
private:
    friend class CrystalFragmentManager;

    Vector3     m_FirstVelocity;
    Physics*    m_pPhysics;
    float       m_Lifetime;
    float       m_ElementPoint;
    float       m_Size;
    float       m_RotSpeed;
};