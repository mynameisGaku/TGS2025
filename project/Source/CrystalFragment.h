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

    /// <summary>
    /// �L�����H
    /// </summary>
    /// <returns>�L���Ȃ�true</returns>
    bool IsActive() const { return m_IsActive; }
private:
    // ���F�B
    friend class CrystalFragmentManager;

    Vector3     m_FirstVelocity;    // �����x �������Ƀ����_���Őݒ�
    Physics*    m_pPhysics;         // �����R���|�[�l���g
    float       m_Lifetime;         // ����
    float       m_ElementPoint;     // �����|�C���g
    float       m_Size;             // �傫��
    float       m_RotSpeed;         // Y����]���x
    bool        m_IsActive;         // �L�����ǂ���
};