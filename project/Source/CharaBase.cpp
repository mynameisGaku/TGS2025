#include "CharaBase.h"
#include "InputManager.h"
#include "EffectManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Stage.h"
#include "StageObjectManager.h"
#include "CharaStamina.h"
#include "CharaHP.h"
#include "CharaDefineRef.h"
#include "Component/Animator.h"
#include "Util/Utils.h"
#include "Timeline.h"
#include "VectorJson.h"

using namespace KeyDefine;

namespace
{
	static const float CATCH_STAMINA_USE = 50.0f;	// キャッチに使うスタミナ（毎秒）
	static const float CATCH_STAMINA_MIN = 20.0f;	// キャッチを開始するのに必要な残スタミナ
	static const float CATCH_TIME = 0.05f;	// 入力一回のキャッチ継続時間
	static const float SLIDE_TIME = 0.05f;	// 入力一回のスライディング継続時間
}

CharaBase::CharaBase()
{
	m_pStamina				= Instantiate<CharaStamina>();
	m_pHP					= Instantiate<CharaHP>();
	m_pPhysics				= nullptr;
	m_MoveSpeed				= 0.0f;
	m_RotSpeed				= 0.0f;
	m_SpeedScale			= 0.0f;
	m_CharaTag				= CHARADEFINE_REF.Tags[0];
	m_Index					= 0;
	m_Animator				= nullptr;
	m_EmoteTimer			= 0.0f;
	m_IsLanding				= true;
	m_SlideTimer			= 0.0f;
	m_EffectTransform		= nullptr;
	m_Timeline				= nullptr;
	m_CanMove				= true;
	m_CanRot				= true;
	m_IsMove				= false;
	m_IsBlocking			= false;
	m_IsAttack				= false;

	m_FSM = new TinyFSM<CharaBase>(this);
	m_SubFSM = new TinyFSM<CharaBase>(this);

#if FALSE
	// この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&CharaBase::StateActionIdle, "StateActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleEmote, "StateActionIdleEmote");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToJump, "StateActionIdleToJump");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToRun, "StateActionIdleToRun");
	m_FSM->RegisterStateName(&CharaBase::StateActionIdleToStandingIdle, "StateActionIdleToStandingIdle");

	m_FSM->RegisterStateName(&CharaBase::StateAirSpin, "StateAirSpin");

	m_FSM->RegisterStateName(&CharaBase::StateCrouchToActionIdle, "StateCrouchToActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateCrouchToRun, "StateCrouchToRun");

	m_FSM->RegisterStateName(&CharaBase::StateDamageToDown, "StateDamageToDown");

	m_FSM->RegisterStateName(&CharaBase::StateFall, "StateFall");
	m_FSM->RegisterStateName(&CharaBase::StateFallToCrouch, "StateFallToCrouch");
	m_FSM->RegisterStateName(&CharaBase::StateFallToRollToIdle, "StateFallToRollToIdle");

	m_FSM->RegisterStateName(&CharaBase::StateRun, "StateRun");
	m_FSM->RegisterStateName(&CharaBase::StateRunToActionIdle, "StateRunToActionIdle");
	m_FSM->RegisterStateName(&CharaBase::StateRunToJump, "StateRunToJump");
	m_FSM->RegisterStateName(&CharaBase::StateRunToSlide, "StateRunToSlide");

	m_FSM->RegisterStateName(&CharaBase::StateSlide, "StateSlide");
	m_FSM->RegisterStateName(&CharaBase::StateSlideToRun, "StateSlideToRun");

	m_FSM->RegisterStateName(&CharaBase::StateStandingIdle, "StateStandingIdle");
	m_FSM->RegisterStateName(&CharaBase::StateStandingIdleEmote, "StateStandingIdleEmote");
	m_FSM->RegisterStateName(&CharaBase::StateStandingIdleToActionIdle, "StateStandingIdleToActionIdle");
#endif // FALSE

	m_FSM->ChangeState(&CharaBase::StateIdle); // ステートを変更
	m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	Function::DeletePointer(m_FSM);
	Function::DeletePointer(m_Timeline);
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pPhysics = GetComponent<Physics>();

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 100.0f;
	m_EffectTransform->position.z = 100.0f;
	m_EffectTransform->rotation.y = Math::DegToRad(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");

	m_Timeline = new Timeline<CharaBase>(this, m_Animator);
	m_Timeline->SetFunction("SetAnimationSpeed", &CharaBase::setAnimationSpeed);
	m_Timeline->SetFunction("MoveToPosition", &CharaBase::moveToPosition);
	m_Timeline->SetFunction("ChangeToRoll", &CharaBase::changeToRoll);
	m_Timeline->SetFunction("EndRoll", &CharaBase::endRoll);
	m_Timeline->SetFunction("SetCanMove", &CharaBase::setCanMove);
	m_Timeline->SetFunction("SetCanRot", &CharaBase::setCanRot);
	m_Timeline->SetFunction("SetVelocity", &CharaBase::setVelocity);
	m_Timeline->LoadJsons("data/Json/Chara/State");

#if FALSE

	//=== 腕アニメーション（ボールを持つ、投げる） ===
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToHold", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "AimToThrow", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "Catch", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "Hold", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "HoldToAim", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "ThrowToActionIdle", AnimOption());

	//=== メインアニメーション ===
	m_Animator->LoadAnim("data/Animation/", "ActionIdle", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleEmote", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToJump", AnimOption().SetIsFixedRoot({ false, true, false }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToRun", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToStandingIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "AirSpin", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "CrouchToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "CrouchToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "DamageToDown", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Fall", AnimOption().SetIsLoop(true).SetIsFixedRoot({ false, true, false }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "FallToCrouch", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "FallToRollToIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "GetBall", AnimOption());

	m_Animator->LoadAnim("data/Animation/", "Run", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "RunToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "RunToJump", AnimOption().SetIsFixedRoot({ false, true, true }).SetOffset(V3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "RunToSlide", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Slide", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "SlideToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "StandingIdle", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "StandingIdleEmote", AnimOption());
	m_Animator->LoadAnim("data/Animation/", "StandingIdleToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

#endif // FALSE
}

void CharaBase::Update() {

	HitGroundProcess();

	// デバッグ機能
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_Timeline->LoadJsons("data/Json/Chara/State");
		m_Animator->DeleteAnimInfos();
		m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");
	}

	m_FSM->Update();
	m_SubFSM->Update();
	m_Timeline->Update();

	static const float MOVE_ACCEL = 0.03f;

	// 移動速度倍率の更新
	if (m_IsMove)
	{
		m_SpeedScale += MOVE_ACCEL;
		if (m_SpeedScale > 1.0f)
		{
			m_SpeedScale = 1.0f;
		}
	}
	else
	{
		m_SpeedScale = 0.0f;
	}

	m_IsMove = false;
	m_IsBlocking = false;
	m_IsAttack = false;

	if (m_pPhysics->velocity.y < -1.0f)
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateFall); // ステートを変更
	}
	else
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
	}

	Object3D::Update();
}

void CharaBase::Draw()
{
	Object3D::Draw();

	if (m_pHP->IsDead())
	{
		DrawFormatString(300, 300, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 320, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 340, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 360, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 380, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 400, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 320, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 340, 0xffffff, "あああああああああああああああああああああああああ");
		DrawFormatString(300, 360, 0xffffff, "あああああああああああああああああああああああああ");
	}
}

void CharaBase::CollisionEvent(const CollisionData& colData) {

	// 当たった相手がキャラクターの場合
	if (colData.Other()->Tag() == ColDefine::Tag::tCharaRed) {

		// 相手の情報
		CharaBase* chara = colData.Other()->Parent<CharaBase>();

		if (chara != nullptr) {
			Vector3 myPos = transform->Global().position;			// 自身の座標
			Vector3 otherPos = chara->transform->Global().position;	// 相手の座標

			Physics* otherPhysics = chara->GetComponent<Physics>();	// 相手の物理挙動

			ColliderCapsule* collider = GetComponent<ColliderCapsule>();	// 当たり判定

			if (m_pPhysics == nullptr || otherPhysics == nullptr || collider == nullptr)
				return;

			// 相手へ向かうベクトル
			Vector3 toVec = otherPos - myPos;
			if (toVec.SquareSize() == 0)
				toVec = Vector3(0, 0, 1);

			// 反発力の強さを定義
			constexpr float REPELLENT_FORCE_SCALE_MAX = 2.0f;
			constexpr float REPELLENT_FORCE_SCALE_MIN = 1.0f;
			const float REPELLENT_RADIUS = collider->Radius() * 4.0f;

			// 反発力
			const Vector3 repellentForce = toVec.Norm() * (REPELLENT_FORCE_SCALE_MIN + (REPELLENT_FORCE_SCALE_MAX - REPELLENT_FORCE_SCALE_MIN) * max(0.0f, 1.0f - toVec.Size() / REPELLENT_RADIUS));

			// 反発力を加える
			m_pPhysics->resistance -= repellentForce;
			otherPhysics->velocity += repellentForce;
		}
	}
}

void CharaBase::HitGroundProcess() {

	// 物理挙動
	Physics* physics = m_pPhysics;
	if (physics == nullptr)
		return;

	// 地形(StageObject)と当たり判定する
	{
		Vector3 p1, p2;

		ColliderCapsule* capsuleCol = GetComponent<ColliderCapsule>();

		p1 = transform->Global().position + V3::SetY(37);
		p2 = capsuleCol->OffsetWorld();

		const float radius = capsuleCol ? capsuleCol->Radius() : 20.0f;

		Vector3 pushVec;
		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			// 押し出しベクトルで位置を補正
			transform->position += pushVec;

			// Y成分が上方向なら接地とみなす
			if (pushVec.y > 0.1f)
			{
				m_IsLanding = true;
				m_pPhysics->velocity.y = 0.0f;
				m_pPhysics->resistance.y = 0.0f;
				m_pPhysics->SetGravity(V3::ZERO);
				m_pPhysics->SetFriction(FRICTION);
			}
			// Y成分が下方向なら頭上ヒット（天井にぶつかった）
			else if (pushVec.y < -0.1f)
			{
				m_pPhysics->velocity.y = min(m_pPhysics->velocity.y, 0.0f);
			}
		}
		else
		{
			// 衝突していなければ、通常の空中挙動へ
			m_IsLanding = false;
			m_pPhysics->SetGravity(GRAVITY);
			m_pPhysics->SetFriction(V3::ZERO);
		}
	}

	if (physics->velocity.y > 0)
	{
		physics->SetGravity(GRAVITY);
		physics->SetFriction(V3::ZERO);
		m_IsLanding = false;
		return;
	}
}

void CharaBase::Move(const Vector3& dir)
{
	m_IsMove = dir.SquareSize() > 0;

	if (m_CanRot)
	{
		float currentRot = transform->rotation.y;	// 現在の向き
		float terminusRot = atan2f(dir.x, dir.z);		// 終点の向き

		// 徐々に終点の向きへ合わせる
		transform->rotation.y = Function::RotAngle(currentRot, terminusRot, m_RotSpeed);
	}

	if (m_CanMove)
	{
		const Vector3 normDir = dir.Norm();
		float deltaTimeMoveSpeed = m_MoveSpeed * m_SpeedScale * Time::DeltaTimeLapseRate();	// 時間経過率を適応した移動速度

		Vector3 velocity = normDir * deltaTimeMoveSpeed * V3::HORIZONTAL;	// スティックの傾きの方向への速度

		// 速度を適応させる
		m_pPhysics->velocity.x = velocity.x;
		m_pPhysics->velocity.z = velocity.z;
	}
}

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
}

void CharaBase::Block()
{
	m_IsBlocking = true;
}

void CharaBase::Attack()
{
	m_IsAttack = true;
}

//========================================================================

void CharaBase::StateIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Idle");
		m_EmoteTimer = CHARADEFINE_REF.EmoteInterval;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();

		m_EmoteTimer -= Time::DeltaTimeLapseRate();
		if (m_EmoteTimer < 0)
		{
			int rand = GetRand(99);
			if (rand < 50)
			{
				m_FSM->ChangeState(&CharaBase::StateIdleEmote_CheckSword); // ステートを変更
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateIdleEmote_Stretch); // ステートを変更
			}
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateIdleEmote_CheckSword(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("IdleEmote_CheckSword");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateIdleEmote_Stretch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("IdleEmote_Stretch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateIdleToBlock(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("IdleToBlock");
		SetMoveSpeed(90.0f);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		blockUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateBlock); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("Run");
		SetMoveSpeed(700.0f);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Timeline->Stop();
		m_Animator->SetPlaySpeed(1.0f);
	}
	break;
	}
}

void CharaBase::StateHeavyAttack1(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("AirSpin");// ダウンロードし直す！
		m_Animator->PlaySub("mixamorig:Spine", "Stab");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsAttack)
		{
			m_FSM->ChangeState(&CharaBase::StateIdleToBlock); // ステートを変更
		}
		else if (m_Animator->IsFinishedSub("mixamorig:Spine"))
		{
			m_FSM->ChangeState(&CharaBase::StateIdle);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig:Spine");
	}
	break;
	}
}

void CharaBase::StateBlock(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Block");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		blockUpdate();

		if (not m_IsBlocking)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockToIdle); // ステートを変更
		}
		else if (m_IsMove)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockWalk);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateBlockToIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("BlockToIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();

		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::StateBlockWalk(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:Spine", "Block");
		m_Timeline->Play("BlockWalk");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		blockUpdate();

		if (not m_IsBlocking)
		{
			m_FSM->ChangeState(&CharaBase::StateBlockToIdle); // ステートを変更
		}
		else if (not m_IsMove)
		{
			m_FSM->ChangeState(&CharaBase::StateBlock);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig:Spine");
		m_Timeline->Stop();
	}
	break;
	}
}

void CharaBase::SubStateNone(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
	}
	break;
	}
}

void CharaBase::SubStateFall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:LeftUpLeg", "Fall");
		m_Animator->PlaySub("mixamorig:RightUpLeg", "Fall");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig:LeftUpLeg");
		m_Animator->StopSub("mixamorig:RightUpLeg");
	}
	break;
	}
}

//========================================================================

void CharaBase::idleUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // ステートを変更
	}
	if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRun); // ステートを変更
	}
	if (m_IsBlocking)
	{
		m_FSM->ChangeState(&CharaBase::StateIdleToBlock);
	}
	canAttackUpdate();
}

void CharaBase::runUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateRunToJump); // ステートを変更
	}
	if (not m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateIdle); // ステートを変更
	}
	if (m_SlideTimer > 0.0f)
	{
		//m_FSM->ChangeState(&CharaBase::StateRunToSlide); // ステートを変更
	}
	if (m_IsBlocking)
	{
		m_FSM->ChangeState(&CharaBase::StateIdleToBlock);
	}
	canAttackUpdate();
}

void CharaBase::blockUpdate()
{
}

void CharaBase::canAttackUpdate()
{
	if (m_IsAttack)
	{
		m_FSM->ChangeState(&CharaBase::StateHeavyAttack1);
	}
}

void CharaBase::setAnimationSpeed(const nlohmann::json& argument)
{
	m_Animator->SetPlaySpeed(argument.at("Speed").get<float>());
}

void CharaBase::moveToPosition(const nlohmann::json& argument)
{
	float progress = argument.at("Progress").get<float>();
	float lastProgress = argument.at("LastProgress").get<float>();
	Vector3 dest = argument.at("Position").get<Vector3>();
	dest = VTransform(dest, transform->RotationMatrix());

	Vector3 add = dest * progress;
	Vector3 sub = dest * lastProgress;

	transform->position += add;
	transform->position -= sub;
}

void CharaBase::changeToRoll(const nlohmann::json& argument)
{
	//m_FSM->ChangeState(&CharaBase::StateRoll); // ステートを変更
}

void CharaBase::endRoll(const nlohmann::json& argument)
{
	if (m_IsMove)
	{
		//m_FSM->ChangeState(&CharaBase::StateRollToRun); // ステートを変更
	}
	else
	{
		//m_FSM->ChangeState(&CharaBase::StateRollToActionIdle); // ステートを変更
	}
}

void CharaBase::setCanMove(const nlohmann::json& argument)
{
	m_CanMove = argument.at("CanMove").get<bool>();
	m_CanRot = m_CanMove;
}

void CharaBase::setCanRot(const nlohmann::json& argument)
{
	m_CanRot = argument.at("CanRot").get<bool>();
}

void CharaBase::setVelocity(const nlohmann::json& argument)
{
	m_pPhysics->velocity = argument.at("Velocity").get<Vector3>();
}
