#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/physics/Physics.h"

/// <summary>
/// �N���X�^�����痎����j�ЃN���X
/// </summary>
class CrystalFragment : public Object3D
{
public:
    enum Element
    {
        eNONE = 0,
        eFIRE,
        eICE,
        eLIGHTNING,
        eWIND,
        eEARTH,
        eMAX
    };
    enum Size
    {
        sSMALL = 0,
        sMIDDLE,
        sLARGE,
        sMAX
    };
public:
    CrystalFragment(uint32_t index = -1, const Element& elem = eNONE, const Size& size = sSMALL);
    void SetRandFirstVelocity();
    ~CrystalFragment();

    void Init(const Element& elem = eNONE, const Size& size = sSMALL);

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

    Vector3         m_FirstVelocity;    // �����x �������Ƀ����_���Őݒ�
    Physics*        m_pPhysics;         // �����R���|�[�l���g
    float           m_Lifetime;         // ����
    float           m_ElementPoint;     // �����|�C���g
    float           m_Size;             // �傫��
    float           m_RotSpeed;         // Y����]���x
    uint32_t        m_Index;            // �����̃C���f�b�N�X
    bool            m_IsActive;         // �L�����ǂ���
    Element         m_Element;          // ����
};

typedef CrystalFragment::Element CrystalElement;
typedef CrystalFragment::Size    CrystalSize;