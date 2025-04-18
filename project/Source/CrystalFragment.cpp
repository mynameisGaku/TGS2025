#include "CrystalFragment.h"
#include "settings_json.h"
#include "Library/FileUtil.h"
#include "CrystalFragmentRef.h"
#include "Util/Utils.h"
#include <Library/time.h>

CrystalFragment::CrystalFragment() :
    Object3D(),
    m_pPhysics(nullptr),
    m_Lifetime(0.0f),
    m_ElementPoint(0.0f),
    m_Size(0.0f)
{
    m_pPhysics = AddComponent<Physics>();
    m_pPhysics->Init(V3::ZERO, FRICTION);

    // ���t�@�����X�����ɏ�����
    auto& ref = CRYSTALFRAGMENT_REF;
    m_Lifetime          = ref.m_LifeTime;
    m_ElementPoint      = ref.m_ElementPoint;
    m_Size              = ref.m_Size;
    m_RotSpeed          = ref.m_RotSpeed;

    // �����������_���Őݒ�
    float rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float x = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX;
    float y = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float z = rand_;

    // ���������ɐݒ�
    x *= 150.0f;
    y *= 80.0f;
    z *= 150.0f;

    // ������ݒ�
    m_FirstVelocity = Vector3(x, y, z);
    m_pPhysics->velocity += m_FirstVelocity;

    // �t���O���Ă�
    m_IsActive = true;
}

CrystalFragment::~CrystalFragment()
{
}

void CrystalFragment::Update()
{
    // �����؂ꂽ�疳����
    if (m_Lifetime <= 0.0f)
    {
        m_IsActive = false;
        return;
    }

    // �������炷
    m_Lifetime -= Time::DeltaTime();
    
    // ��]
    m_pPhysics->angularVelocity.y = m_RotSpeed;

    // �ʒu�X�V
    m_pPhysics->Update();
    m_pPhysics->velocity.x *= 0.8f; // ���C
    m_pPhysics->velocity.y *= 0.8f; // ���C
    m_pPhysics->velocity.z *= 0.8f; // ���C

    // ��U Y<0 �ɂȂ����甽��
    // ToDo : �����Ə��Ɠ����蔻�肷��
    if (transform->position.y <= 0.0f)
    {
        transform->position.y = 0.0f;
        m_pPhysics->velocity.y *= -1.0f;
    }
}

void CrystalFragment::Draw()
{
    // �����؂ꂽ�������
    if (m_Lifetime <= 0.0f)
        return;

    // �J�v�Z���ŉ��`��
    DrawCapsule3D(
        transform->position,
        transform->position + Vector3(0.0f, 100.0f, 0.0f),
        m_Size,
        32,
        0xffffff,
        0xff0000,
        true
    );
}