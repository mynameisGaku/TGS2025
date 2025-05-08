#include "Ball.h"
#include "Library/resourceLoader.h"
#include "Component/Physics.h"
#include "BallRef.h"
#include "Component/ColliderCapsule.h"
#include "Stage.h"
#include "BloomManager.h"
#include "CharaDefineRef.h"
#include "CharaBase.h"
#include "EffectManager.h"


namespace
{
    static const float BALL_MODEL_RADIUS = 83.951f;
    static const float BALL_RADIUS = 70.0f;
    static const float BALL_SCALE = BALL_RADIUS / BALL_MODEL_RADIUS;
    static const float BALL_COLOR_RADIUS = 90.0f;
}

Ball::Ball()
{
    Object3D::SetModel(ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1"));

    transform->scale = V3::ONE * BALL_SCALE;
    m_Physics = Object3D::AddComponent<Physics>();
    m_Physics->Init(BALL_REF.GravityDefault, BALL_REF.FrictionDefault);

    m_State = S_OWNED;
    m_Owner = nullptr;
    m_Collider = nullptr;
    m_CharaTag = CHARADEFINE_REF.Tags[0];

    m_IsHoming = false;
}

Ball::~Ball()
{
    if (m_Owner != nullptr && m_Owner->LastBall() == this)
    {
        m_Owner->SetLastBall(nullptr);
    }
}

void Ball::Init(std::string charaTag)
{
    m_Collider = Object3D::AddComponent<ColliderCapsule>();

    ColDefine::ColBaseParam param;
    param.trs.scale = V3::ONE * BALL_COLOR_RADIUS / BALL_SCALE * 2;

    if (charaTag == "Red")
    {
        param.tag = ColDefine::Tag::tBallRed;
        param.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain };
    }
    else if (charaTag == "Blue")
    {
        param.tag = ColDefine::Tag::tBallBlue;
        param.targetTags = { ColDefine::Tag::tCharaRed, ColDefine::Tag::tCatchRed, ColDefine::Tag::tTerrain };
    }
    else
    {
        param.tag = ColDefine::Tag::tBallRed;
        param.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tCatchBlue, ColDefine::Tag::tTerrain };
    }

    m_Collider->SetOffset(V3::ZERO);
    m_Collider->BaseInit(param);
    m_CharaTag = charaTag;
}

void Ball::Update()
{
    if (m_IsHoming)
    {
        // ホーミングでゴールまで線形補間 (物理未使用)
        {
            HomingProcess();
        }

        m_Physics->SetIsActive(false);
    }
    Object3D::Update();

    collisionToGround();
}

void Ball::Draw()
{
    Object3D::Draw();
}

void Ball::Throw(const Vector3& velocity)
{
    m_State = S_THROWN;
    setVelocity(velocity * BALL_REF.SpeedDefault);
    m_Owner = nullptr;
}

void Ball::Throw(const Vector3& velocity, CharaBase* owner)
{
    Throw(velocity);
    m_Owner = owner;
}

void Ball::ThrowHoming(const Vector3& velocity, CharaBase* owner)
{
    m_State = S_THROWN;
    m_Owner = owner;
    m_HomingPosition = transform->position + V3::SetY(100.0f);
    m_HomingTarget = Vector3(0, 0, 1000);
    m_Physics->velocity = velocity;

    // ターゲット位置と現在位置からちょうどいい時間を計算
    Vector3 diff = m_HomingTarget - m_HomingPosition;
    m_HomingPeriod = diff.Size() / m_Physics->velocity.Size();
    m_Physics->SetGravity(V3::ZERO);
    m_Physics->SetIsActive(false);
    m_IsHoming = true;
}

void Ball::CollisionEvent(const CollisionData& colData)
{
    if (m_State == S_THROWN)
    {
        if (m_IsHoming) HomingDeactivate();

        m_Physics->velocity = m_Physics->FlatVelocity() * -0.5f + Vector3(0, 20, 0);
        m_State = S_LANDED;
        if (m_Owner->LastBall() == this)
        {
            if (m_CharaTag == "Blue")
            {
                EffectManager::Play3D("Hit_Blue.efk", *transform->Copy(), "Hit_Blue" + m_CharaTag);
            }
            else
            {
                EffectManager::Play3D("Hit_Red.efk", *transform->Copy(), "Hit_Red" + m_CharaTag);
            }

            m_Owner->SetLastBall(nullptr);
        }
    }
}

void Ball::collisionToGround()
{
    Vector3 hitPos;
    bool hit = Stage::ColCheckGround(transform->position + V3::SetY(BALL_RADIUS), transform->position - V3::SetY(BALL_RADIUS), &hitPos);
    if (hit)
    {
        if (m_IsHoming) HomingDeactivate();

        // Y方向に跳ね返る
        transform->position = hitPos + V3::SetY(BALL_RADIUS);
        m_Physics->velocity.y *= -BALL_REF.BouncinessDefault;

        // 転がっていく処理
        float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
        transform->rotation.y = forwardRad;

        m_Physics->velocity.x *= 0.99f;
        m_Physics->velocity.z *= 0.99f;
        m_Physics->angularVelocity.x = m_Physics->FlatVelocity().Size() * 0.01f;
    }
}

void Ball::HomingProcess()
{
    Vector3 acceleration = m_HomingTarget - transform->position;
    Vector3 diff = m_HomingTarget - m_HomingPosition;

    acceleration += (diff - m_Physics->velocity * m_HomingPeriod) * 2.0f / (m_HomingPeriod * m_HomingPeriod);

    m_HomingPeriod -= Time::DeltaTimeLapseRate();
    if (m_HomingPeriod < 0.0f)
    {
        HomingDeactivate();
    }

    m_Physics->velocity += acceleration * Time::DeltaTimeLapseRate();
    m_HomingPosition += m_Physics->velocity * Time::DeltaTimeLapseRate();
    transform->position = m_HomingPosition;
}

void Ball::HomingDeactivate()
{
    m_Physics->velocity *= Time::DeltaTimeLapseRate();
    m_Physics->SetIsActive(true);
    m_Physics->SetGravity(BALL_REF.GravityDefault);
    // 物理にホーミングの情報を引き継ぐ
    m_IsHoming = false;
}

void Ball::setVelocity(const Vector3& velocity)
{
    m_Physics->velocity = velocity;
}
