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

    // リファレンスを元に初期化
    auto& ref = CRYSTALFRAGMENT_REF;
    m_Lifetime          = ref.m_LifeTime;
    m_ElementPoint      = ref.m_ElementPoint;
    m_Size              = ref.m_Size;
    m_RotSpeed          = ref.m_RotSpeed;

    // 初速をランダムで設定
    float rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float x = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX;
    float y = rand_;
    rand_ = GetRand(INT_MAX) / (float)INT_MAX - 0.5f;
    float z = rand_;

    // いい感じに設定
    x *= 150.0f;
    y *= 80.0f;
    z *= 150.0f;

    // 初速を設定
    m_FirstVelocity = Vector3(x, y, z);
    m_pPhysics->velocity += m_FirstVelocity;

    // フラグ立てる
    m_IsActive = true;
}

CrystalFragment::~CrystalFragment()
{
}

void CrystalFragment::Update()
{
    // 寿命切れたら無効化
    if (m_Lifetime <= 0.0f)
    {
        m_IsActive = false;
        return;
    }

    // 寿命減らす
    m_Lifetime -= Time::DeltaTime();
    
    // 回転
    m_pPhysics->angularVelocity.y = m_RotSpeed;

    // 位置更新
    m_pPhysics->Update();
    m_pPhysics->velocity.x *= 0.8f; // 摩擦
    m_pPhysics->velocity.y *= 0.8f; // 摩擦
    m_pPhysics->velocity.z *= 0.8f; // 摩擦

    // 一旦 Y<0 になったら反発
    // ToDo : ちゃんと床と当たり判定する
    if (transform->position.y <= 0.0f)
    {
        transform->position.y = 0.0f;
        m_pPhysics->velocity.y *= -1.0f;
    }
}

void CrystalFragment::Draw()
{
    // 寿命切れたら消える
    if (m_Lifetime <= 0.0f)
        return;

    // カプセルで仮描画
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