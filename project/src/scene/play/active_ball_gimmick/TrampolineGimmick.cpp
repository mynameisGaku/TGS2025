#include "TrampolineGimmick.h"
#include <src/common/component/collider/ColliderBase.h>
#include <src/common/component/physics/Physics.h>
#include <src/scene/play/ball/Ball.h>
#include <src/scene/play/chara/Chara.h>

TrampolineGimmick::TrampolineGimmick()
{
}

TrampolineGimmick::~TrampolineGimmick()
{
}

void TrampolineGimmick::Init(const TRAMPOLINE_DESC& desc)
{
    *transform = desc.transform.Global();
    m_RepulsionForce = desc.repulsion_force;

    // ‰ñ“]‚©‚ç–@ü‚ð‹‚ß‚é
    const Vector3 normal = Vector3(0, 1, 0);
    m_SurfaceNormal = normal * transform->RotationMatrix();
}

void TrampolineGimmick::Update()
{
    Object3D::Update();
}

void TrampolineGimmick::Draw()
{
    DrawSphere3D(transform->position, 100.0f, 128, 0xffff00, 0x000000, true);
    Object3D::Draw();
}

void TrampolineGimmick::CollisionEvent(const CollisionData& colData)
{
    auto other = colData.Other();

    if (other->Parent<Ball>() != nullptr)
    {
        auto ball = other->Parent<Ball>();
        OnBallHit(ball);
    }
    else if (other->Parent<Chara>() != nullptr)
    {
        auto chara = other->Parent<Chara>();
        OnCharaContact(chara);
    }
}

void TrampolineGimmick::OnBallHit(Ball* pBall)
{
    ReflectBall(pBall);
}

void TrampolineGimmick::OnCharaContact(Chara* pChara)
{
    ReflectChara(pChara);
}

void TrampolineGimmick::ReflectBall(Ball* pBall)
{
    auto physics = pBall->GetComponent<Physics>();
    bounce(physics, type::ball);
}

void TrampolineGimmick::ReflectChara(Chara* pChara)
{
	auto physics = pChara->GetComponent<Physics>();
	bounce(physics, type::chara);
}

void TrampolineGimmick::bounce(Physics* pPhys, const type& _type)
{
    auto reflection = [&](Vector3 v_in, Vector3 normal, float scale){
        float refl = v_in - 2.0f * (Vector3::Dot(v_in, normal)) * normal;
        pPhys->velocity = refl * m_RepulsionForce;
    };

    Vector3 vel = pPhys->velocity;
    float power = 0.0f;

    switch (_type)
    {
    case type::ball:
        power = m_RepulsionForce;
        break;
    case type::chara:
        power = m_RepulsionForce * 0.5f;
        break;
    default:
        power = m_RepulsionForce;
        break;
	}
	reflection(vel, m_SurfaceNormal, power);
}
