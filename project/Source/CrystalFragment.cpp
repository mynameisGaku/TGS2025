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

    // ƒŠƒtƒ@ƒŒƒ“ƒX‚ðŒ³‚É‰Šú‰»
    auto& ref = CRYSTALFRAGMENT_REF;
    m_Lifetime          = ref.m_LifeTime;
    m_ElementPoint      = ref.m_ElementPoint;
    m_Size              = ref.m_Size;
    m_RotSpeed          = ref.m_RotSpeed;

    // ‰‘¬‚ðƒ‰ƒ“ƒ_ƒ€‚ÅÝ’è
    float rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float x = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX;
    float y = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float z = rand_;

    x *= 150.0f;
    y *= 80.0f;
    z *= 150.0f;

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
    
    // ‰ñ“]
    m_pPhysics->angularVelocity.y = m_RotSpeed;

    // ˆÊ’uXV
    m_pPhysics->Update();
    m_pPhysics->velocity.x *= 0.8f; // –€ŽC
    m_pPhysics->velocity.y *= 0.8f; // –€ŽC
    m_pPhysics->velocity.z *= 0.8f; // –€ŽC

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