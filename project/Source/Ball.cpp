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
#include "StageObjectManager.h"


namespace
{
	static const float BALL_MODEL_RADIUS = 83.951f;
	static const float BALL_RADIUS = 30.0f;
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
        m_Physics->SetIsActive(false);
        HomingProcess();
    }
    else
    {
        if (m_Collider)
        {
            Vector3 p1 = transform->Global().position;
            Vector3 p2 = m_Collider->OffsetWorld();
            float radius = BALL_RADIUS;

            Vector3 pushVec;
            if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
            {
                transform->position += pushVec;

                // 押し出し方向
                Vector3 normal = pushVec.Norm();

                // 速度を反射させる
                float bounciness = BALL_REF.BouncinessDefault;
                Vector3 vel = m_Physics->velocity;

                float dot = VDot(vel, normal);
                if (dot < 0.0f) // 内側からの衝突のみ反射
                {
                    Vector3 reflectVel = vel - normal * (1.0f + bounciness) * dot;
                    m_Physics->velocity = reflectVel;
                }

                // Y方向の反発時に転がり回転も発生
                if (abs(normal.y) > 0.5f)
                {
                    float forwardRad = atan2f(m_Physics->velocity.x, m_Physics->velocity.z);
                    transform->rotation.y = forwardRad;
                    m_Physics->angularVelocity.x = m_Physics->FlatVelocity().Size() * 0.01f;
                }
            }
        }
    }

    Object3D::Update();
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
    // ---- ホーミング補間 ----
    Vector3 acceleration = m_HomingTarget - transform->position;
    Vector3 diff = m_HomingTarget - m_HomingPosition;

    acceleration += (diff - m_Physics->velocity * m_HomingPeriod) * 2.0f / (m_HomingPeriod * m_HomingPeriod);

    m_HomingPeriod -= Time::DeltaTimeLapseRate();
    if (m_HomingPeriod < 0.0f)
    {
        HomingDeactivate();
        return;
    }

    m_Physics->velocity += acceleration * Time::DeltaTimeLapseRate();
    m_HomingPosition += m_Physics->velocity * Time::DeltaTimeLapseRate();

    // ---- 押し出し + 跳ね返り処理 ----
    if (m_Collider)
    {
        Vector3 p1 = m_HomingPosition;
        Vector3 p2 = m_Collider->OffsetWorld();  // ホーミング中は本来位置からズレるが、近似可

        float radius = BALL_RADIUS;
        Vector3 pushVec;

        if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
        {
            HomingDeactivate();

            m_HomingPosition += pushVec;

            Vector3 normal = pushVec.Norm();
            float dot = VDot(m_Physics->velocity, normal);

            if (dot < 0.0f)
            {
                const float bounciness = BALL_REF.BouncinessDefault;
                const float damping = 0.85f; // ホーミング中の減衰

                Vector3 reflectVel = m_Physics->velocity - normal * (1.0f + bounciness) * dot;
                m_Physics->velocity = reflectVel * damping;
            }
        }
    }

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
