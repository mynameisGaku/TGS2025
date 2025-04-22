#pragma once
#include "object3d.h"
#include "Component/Physics.h"

/// <summary>
/// �N���X�^�����痎����j�ЃN���X
/// </summary>
class CrystalFragment : public Object3D
{
public:
    CrystalFragment(uint32_t index = -1);
    void SetRandFirstVelocity();
    ~CrystalFragment();

    void Update() override;
    void Draw() override;

    /// <summary>
    /// �L�����H
    /// </summary>
    /// <returns>�L���Ȃ�true</returns>
    bool IsActive() const { return m_IsActive; }

    void SetIsActive(bool flag) { m_IsActive = flag; }
private:
    // ���F�B
    friend class CrystalFragmentManager;

    Vector3     m_FirstVelocity;    // �����x �������Ƀ����_���Őݒ�
    Physics*    m_pPhysics;         // �����R���|�[�l���g
    float       m_Lifetime;         // ����
    float       m_ElementPoint;     // �����|�C���g
    float       m_Size;             // �傫��
    float       m_RotSpeed;         // Y����]���x
    uint32_t    m_Index;            // �����̃C���f�b�N�X
    bool        m_IsActive;         // �L�����ǂ���
};