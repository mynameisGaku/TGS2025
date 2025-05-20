#include "src/scene/play/crystal/CrystalFragment.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/FileUtil.h"
#include "src/reference/crystal/CrystalFragmentRef.h"
#include "src/util/Utils.h"
#include "src/util/time/GameTime.h"

CrystalFragment::CrystalFragment(uint32_t index, const Element& elem, const Size& size) :
    Object3D(),
    m_pPhysics(nullptr),
    m_Lifetime(0.0f),
    m_ElementPoint(0.0f),
    m_Size(0.0f),
    m_Index(index),
    m_Element(elem)
{
}

void CrystalFragment::SetRandFirstVelocity()
{
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
}

CrystalFragment::~CrystalFragment()
{
    std::string output = "cry delete : " + std::to_string(m_Index) + "\n";
    OutputDebugString(output.c_str());
}

void CrystalFragment::Init(const Element& elem, const Size& size)
{
    m_pPhysics = AddComponent<Physics>();
    m_pPhysics->Init(Vector3::Zero, FRICTION);

    m_Element = elem;

    // ���t�@�����X�����ɏ�����
    auto& ref = CRYSTALFRAGMENT_REF;
    m_Lifetime = ref.m_Refs[size].LifeTime;
    m_ElementPoint = ref.m_Refs[size].ElementPoint;
    m_Size = ref.m_Refs[size].Size;
    m_RotSpeed = ref.m_Refs[size].RotSpeed;

    // �t���O���Ă�
    m_IsActive = true;
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
    m_Lifetime -= GTime.deltaTime;
    
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

    int color = 0x000000;
    switch (m_Element)
    {
    case eFIRE:
        color = 0xff0000;
        break;
    case eICE:
        color = 0x00ffff;
        break;
    case eLIGHTNING:
        color = 0xffff00;
        break;
    case eWIND:
        color = 0x00ff00;
        break;
    case eEARTH:
        color = 0x0000ff;
        break;
    case eNONE:
        color = 0xffffff;
        break;
    default:
        color = 0xffffff;
        break;
    }


    // �J�v�Z���ŉ��`��
    DrawCapsule3D(
        transform->position,
        transform->position + Vector3(0.0f, m_Size, 0.0f),
        m_Size,
        32,
        color,
        color,
        true
    );
}