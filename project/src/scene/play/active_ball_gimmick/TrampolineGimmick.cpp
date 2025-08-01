#include "TrampolineGimmick.h"
#include <src/common/component/collider/ColliderBase.h>
#include <src/common/component/physics/Physics.h>
#include <src/scene/play/ball/Ball.h>
#include <src/scene/play/chara/Chara.h>
#include <src\common\component\collider\ColliderSphere.h>
#include <src\util\object3D\Object3D.h>

static const float DEFAULT_REPULSION_FORCE = 300.0f;

TrampolineGimmick::TrampolineGimmick()
{
}

TrampolineGimmick::~TrampolineGimmick()
{
}

void TrampolineGimmick::Init(const TRAMPOLINE_DESC& desc, const ColDefine::ColBaseParam& col)
{
	*transform = desc.transform.Global();
	m_RepulsionForce = desc.repulsion_force;

	// ‰ñ“]‚©‚ç–@ü‚ð‹‚ß‚é
	const Vector3 normal = Vector3(0, 1, 0);
	m_SurfaceNormal = normal * transform->RotationMatrix();
	m_SurfaceNormal.Normalize();

	auto collider = AddComponent<ColliderSphere>();
	collider->BaseInit(col);
}

void TrampolineGimmick::Update()
{
	Object3D::Update();
}

void TrampolineGimmick::Draw()
{
	auto col = GetComponent<ColliderSphere>();
	//DrawSphere3D(col->Global().position, col->Radius(), 64, 0xffffff, 0x000000, true);
	Object3D::Draw();
}

void TrampolineGimmick::CollisionEvent(const CollisionData& colData)
{
	auto other = colData.Other();


	// ‚±‚±‚É‚«‚½
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
	float power = 0.0f;

	switch (_type)
	{
	case type::ball:
		power = m_RepulsionForce;
		break;
	case type::chara:
		power = m_RepulsionForce * 0.9f;
		break;
	default:
		power = m_RepulsionForce;
		break;
	}

	Vector3 vel = pPhys->velocity;
	pPhys->velocity = Vector3::Zero;

	// –@ü‚Ì•û‚É‚Ô‚Á”ò‚Î‚·
	Vector3 reflection = (m_SurfaceNormal * power * DEFAULT_REPULSION_FORCE) 
		//* GTime.deltaTime
		;
	pPhys->velocity = vel + reflection;
}
