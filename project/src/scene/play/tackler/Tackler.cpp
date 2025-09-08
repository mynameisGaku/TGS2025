#include "src/scene/play/tackler/Tackler.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/scene/play/chara/Chara.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/common/stage/StageObjectManager.h"
#include <src\common\camera\CameraManager.h>

Tackler::Tackler()
{
	m_Collider = nullptr;
	m_Parent = nullptr;
}

Tackler::~Tackler()
{
}

void Tackler::Init(std::string tag)
{
	m_Collider = AddComponent<ColliderCapsule>();

	ColDefine::ColBaseParam param;

	if (tag == "Red")
	{
		param.tag = ColDefine::Tag::tRed;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tChara, ColDefine::Tag::tBlue };
	}
	else if (tag == "Blue")
	{
		param.tag = ColDefine::Tag::tBlue;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tChara, ColDefine::Tag::tRed };
	}
	else
	{
		param.tag = ColDefine::Tag::tRed;
		param.targetTags = { ColDefine::Tag::tBall, ColDefine::Tag::tChara, ColDefine::Tag::tBlue };
	}

	m_Collider->BaseInit(param);
	//m_Collider->SetDraw(true);
}

void Tackler::Update()
{
	Object3D::Update();
}

void Tackler::Draw()
{
	Object3D::Draw();
}

void Tackler::CollisionEvent(const CollisionData& colData)
{
	// ‚ ‚½‚Á‚½‘ŠŽè‚ªƒLƒƒƒ‰‚È‚ç
	Chara* chara = colData.Other()->Parent<Chara>();
	if (chara not_eq nullptr)
	{
		if (chara == m_Parent)
			return;

		if (chara->GetCharaTag() == m_Parent->GetCharaTag())
		{
			m_Parent->GetTackle(chara->transform->position, CHARADEFINE_REF.TackleForce_Horizontal, CHARADEFINE_REF.TackleForce_Vertical, true);
			return;
		}

		if (chara->IsInvincible())
			return;

		GameTime::SetHitStopSeconds(10.0f / 60.0f);
		
		auto camera = CameraManager::GetCamera(chara->GetIndex());
		camera->GetComponent<Shake>()->SetParam({ Shake::Type::tAll }, 50.0f, Vector3(0.1f, 0.1f, 0.1f), 10.0f / 60.0f);

		camera = CameraManager::GetCamera(m_Parent->GetIndex());
		camera->GetComponent<Shake>()->SetParam({ Shake::Type::tAll }, 50.0f, Vector3(0.1f, 0.1f, 0.1f), 10.0f / 60.0f);

		chara->GetTackle(m_Parent->transform->position, CHARADEFINE_REF.TackleForce_Horizontal, CHARADEFINE_REF.TackleForce_Vertical, false);

		return;
	}
}

void Tackler::SetColliderActive(bool isActive)
{
	m_Collider->SetIsActive(isActive);
}

bool Tackler::IsColliderActive() const
{
	return m_Collider->IsActive();
}
