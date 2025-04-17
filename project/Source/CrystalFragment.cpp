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
    m_pPhysics->Init(GRAVITY, FRICTION);

    // リファレンスを元に初期化
    auto& ref = CRYSTALFRAGMENT_REF;
    m_Lifetime          = ref.m_LifeTime;
    m_ElementPoint      = ref.m_ElementPoint;
    m_Size              = ref.m_Size;
    m_RotSpeed          = ref.m_RotSpeed;

    // 初速をランダムで設定
    float x = static_cast<float>(GetRand(INT_MAX)) / static_cast<float>(INT_MAX);
    float y = fabsf(static_cast<float>(GetRand(INT_MAX)) / static_cast<float>(INT_MAX));
    float z = static_cast<float>(GetRand(INT_MAX)) / static_cast<float>(INT_MAX);

    x *= 50.0f;
    y *= 50.0f;
    z *= 50.0f;

    m_FirstVelocity = Vector3(x, y, z);
    m_pPhysics->velocity += m_FirstVelocity;
}

CrystalFragment::~CrystalFragment()
{
}

void CrystalFragment::Update()
{
    if (m_Lifetime <= 0.0f)
        return;

    m_Lifetime -= Time::DeltaTime();
    
    // 回転
    m_pPhysics->angularVelocity.y = m_RotSpeed;

    // 位置更新
    m_pPhysics->Update();

    if (transform->position.y <= 0.0f)
    {
        transform->position.y = 0.0f;
        m_pPhysics->velocity.y *= -0.4f;
    }
}

void CrystalFragment::Draw()
{
    if (m_Lifetime <= 0.0f)
        return;

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