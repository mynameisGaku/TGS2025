#include "CharaBase.h"
#include "InputManager.h"
#include "EffectManager.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Stage.h"
#include "StageObjectManager.h"
#include "CharaStamina.h"
#include "CharaHP.h"
#include "Ball.h"
#include "BallManager.h"
#include "Catcher.h"
#include "CharaDefineRef.h"
#include "Component/Animator.h"
#include "Util/Utils.h"
#include "Timeline.h"

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
	m_pBall					= nullptr;
	m_pLastBall				= nullptr;
	m_pPhysics				= nullptr;
	m_BallChargeRate		= 0.0f;
	m_IsCharging			= false;
	m_MoveSpeed				= 0.0f;
	m_RotSpeed				= 0.0f;
	m_ChargeRateWatchDog	= 0.0f;
	m_CatchTimer			= 0.0f;
	m_CharaTag				= CHARADEFINE_REF.Tags[0];
	m_Catcher				= nullptr;
	m_Index					= 0;
	m_Animator				= nullptr;
	m_EmoteTimer			= 0.0f;
	m_IsLanding				= true;
	m_SlideTimer			= 0.0f;
	m_EffectTransform		= nullptr;
	m_pBallManager			= nullptr;

	m_FSM = new TinyFSM<CharaBase>(this);

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

	m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	Function::DeletePointer(m_FSM);
	Function::DeletePointer(m_Timeline);

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	m_Catcher->transform->scale = V3::ONE * CHARADEFINE_REF.CatchRadius * 2;
	m_Catcher->transform->SetParent(transform);
	m_Catcher->Init(tag);
	m_Catcher->SetColliderActive(false);
	m_Catcher->SetParent(this);

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 100.0f;
	m_EffectTransform->position.z = 100.0f;
	m_EffectTransform->rotation.y = Math::DegToRad(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig9:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");

	m_Timeline = new Timeline<CharaBase>(this, m_Animator);
	m_Timeline->SetFunction("SetAnimationSpeed", &CharaBase::setAnimationSpeed);
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

	if (m_CatchTimer > 0.0f)
	{
		m_CatchTimer -= Time::DeltaTimeLapseRate();
		if (m_CatchTimer < 0.0f)
		{
			m_CatchTimer = 0.0f;
			m_Catcher->SetColliderActive(false);
		}
		else
		{
			m_Catcher->SetColliderActive(true);
			if (m_EffectTransform != nullptr)
			{
				EffectManager::Play3D("Catch_Ready_Single_Dust.efk", m_EffectTransform->Global(), "Catch_Ready_Single_Dust" + m_CharaTag);
				EffectManager::Play3D("Catch_Ready_Single_Tornado.efk", m_EffectTransform->Global(), "Catch_Ready_Single_Tornado" + m_CharaTag);
			}
		}

		m_pStamina->Use(CATCH_STAMINA_USE * Time::DeltaTimeLapseRate());
	}
	else
	{
		EffectManager::Stop("Catch_Ready_Single_Dust.efk", "Catch_Ready_Single_Dust" + m_CharaTag);
		EffectManager::Stop("Catch_Ready_Single_Tornado.efk", "Catch_Ready_Single_Tornado" + m_CharaTag);
	}

	if (m_SlideTimer > 0.0f)
	{
		m_SlideTimer -= Time::DeltaTimeLapseRate();
		if (m_SlideTimer < 0.0f)
		{
			m_SlideTimer = 0.0f;
			m_pPhysics->SetFriction(FRICTION);
		}
		else
		{
			m_pPhysics->SetFriction(FRICTION / 10.0f);
		}
	}

	m_FSM->Update();

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
	// ボールの場合
	else if (colData.Other()->Tag() == ColDefine::Tag::tBallBlue || colData.Other()->Tag() == ColDefine::Tag::tBallRed)
	{
		// 相手の情報
		Ball* ball = colData.Other()->Parent<Ball>();

		if (ball->GetCharaTag() != m_CharaTag)
		{
			getHit(ball);
			m_pHP->Damage_UseDefault();
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

	if (m_pBall)
	{
		m_ChargeRateWatchDog -= Time::DeltaTime();
		if (m_ChargeRateWatchDog < 0.0f)
		{
			m_ChargeRateWatchDog = 0.0f;
			m_IsCharging = false;
		}

		m_pBall->transform->position = transform->Global().position;
		m_pBall->transform->rotation = transform->Global().rotation;
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
	if (m_SlideTimer > 0.0f) return;

	float currentRot = transform->rotation.y;	// 現在の向き
	float terminusRot = atan2f(dir.x, dir.z);		// 終点の向き

	// 徐々に終点の向きへ合わせる
	transform->rotation.y = Function::RotAngle(currentRot, terminusRot, m_RotSpeed);

	float deltaTimeMoveSpeed = m_MoveSpeed * Time::DeltaTimeLapseRate();	// 時間経過率を適応した移動速度

	Vector3 velocity = dir * deltaTimeMoveSpeed * V3::HORIZONTAL;	// スティックの傾きの方向への速度

	// 速度を適応させる
	m_pPhysics->velocity.x = velocity.x;
	m_pPhysics->velocity.z = velocity.z;
}

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
}

void CharaBase::Slide()
{
	if (m_FSM->GetCurrentState() == &CharaBase::StateSlideToRun)
	{
		return;
	}

	m_SlideTimer = SLIDE_TIME;
}

void CharaBase::ThrowBall(const Vector3& velocity)
{
	if (m_pBall == nullptr)
		return;

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // ステートを変更
}

void CharaBase::ThrowBallForward()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + V3::SetY(0.4f);

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // ステートを変更
}

void CharaBase::ThrowHomingBall()
{
    if (m_pBall == nullptr)
        return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + V3::SetY(0.4f);
    //m_pBall->ThrowHoming(velocity * (1.0f + m_BallChargeRate), this);
    m_pBall->ThrowHoming(velocity * 3000.0f, this);
    m_pLastBall = m_pBall;
    m_pBall = nullptr;
}

void CharaBase::GenerateBall()
{
	m_ChargeRateWatchDog = 0.1f;

	if (m_pBall != nullptr)
	{
		m_IsCharging = true;
		m_BallChargeRate += Time::DeltaTime();
		return;
	}

	if(m_pBallManager == nullptr)
	{
		m_pBallManager = FindGameObject<BallManager>();
	}

	m_pBall = m_pBallManager->CreateBall(transform->Global().position);

	if (m_pBall == nullptr)
		return;

	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	//m_pBall->SetParent(this);
	m_pBall->Init(m_CharaTag);

	m_FSM->ChangeState(&CharaBase::StateGetBall); // ステートを変更
}

void CharaBase::TeleportToLastBall()
{
	if (m_pLastBall == nullptr) return;
	transform->position = m_pLastBall->transform->position;

	// ToDo:消える演出
	m_pLastBall->DestroyMe();

	GenerateBall();

	m_pLastBall = nullptr;
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_FSM->ChangeState(&CharaBase::StateAirSpin);
}

void CharaBase::Catch()
{
	if (m_pStamina->GetCurrent() > CATCH_STAMINA_MIN)
	{
		m_CatchTimer = CATCH_TIME;
		m_FSM->ChangeState(&CharaBase::StateCatch); // ステートを変更
	}
}

//========================================================================

void CharaBase::StateActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("ActionIdle");
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
				m_FSM->ChangeState(&CharaBase::StateActionIdleToStandingIdle); // ステートを変更
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateActionIdleEmote); // ステートを変更
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

void CharaBase::StateActionIdleEmote(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("ActionIdleEmote");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateActionIdleToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("ActionIdleToJump");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
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

void CharaBase::StateActionIdleToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("ActionIdleToRun");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // ステートを変更
			m_Animator->Play("Run");
			//m_Animator->SetCurrentFrame(3.0f);
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

void CharaBase::StateActionIdleToStandingIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("ActionIdleToStandingIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateStandingIdleEmote); // ステートを変更
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

void CharaBase::StateAirSpin(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("AirSpin");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
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

void CharaBase::StateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Catch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_CatchTimer <= 0.0f)
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateCrouchToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("CrouchToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateCrouchToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("CrouchToRun");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // ステートを変更
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

void CharaBase::StateDamageToDown(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("DamageToDown");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToActionIdle); // ステートを変更
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

void CharaBase::StateFall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Fall");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsLanding)
		{
			if (m_pPhysics->FlatVelocity().SquareSize() > 0)
			{
				m_FSM->ChangeState(&CharaBase::StateFallToRollToIdle); // ステートを変更
			}
			else
			{
				m_FSM->ChangeState(&CharaBase::StateFallToCrouch); // ステートを変更
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

void CharaBase::StateFallToCrouch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("FallToCrouch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_pPhysics->FlatVelocity().SquareSize() > 0)
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToRun); // ステートを変更
		}
		else
		{
			m_FSM->ChangeState(&CharaBase::StateCrouchToActionIdle); // ステートを変更
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

void CharaBase::StateFallToRollToIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("FallToRollToIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateGetBall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("GetBall");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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
		m_Animator->Play("Run");
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
	}
	break;
	}
}

void CharaBase::StateRunToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("RunToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateRunToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("RunToJump");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
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

void CharaBase::StateRunToSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("RunToSlide");
		m_pPhysics->velocity = m_pPhysics->UpVelocity() + m_pPhysics->FlatVelocity() * 2.0f;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		slideUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateSlide); // ステートを変更
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

void CharaBase::StateSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Slide");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		slideUpdate();
		if (m_SlideTimer <= 0.0f)
		{
			m_FSM->ChangeState(&CharaBase::StateSlideToRun); // ステートを変更
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

void CharaBase::StateSlideToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("SlideToRun");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // ステートを変更
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

void CharaBase::StateStandingIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("StandingIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
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

void CharaBase::StateStandingIdleEmote(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("StandingIdleEmote");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateStandingIdleToActionIdle); // ステートを変更
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

void CharaBase::StateStandingIdleToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("StandingIdleToActionIdle");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		idleUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

void CharaBase::StateAimToThrow(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("AimToThrow");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
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

//========================================================================

void CharaBase::idleUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // ステートを変更
	}
	else if (m_pPhysics->FlatVelocity().SquareSize() > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToRun); // ステートを変更
	}
}

void CharaBase::runUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // ステートを変更
	}
	if (m_pPhysics->FlatVelocity().SquareSize() <= 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToActionIdle); // ステートを変更
	}
	if (m_SlideTimer > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToSlide); // ステートを変更
	}
}

void CharaBase::slideUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // ステートを変更
	}
}

void CharaBase::getHit(Ball* hit)
{
	m_FSM->ChangeState(&CharaBase::StateDamageToDown);

	Vector3 dif = hit->transform->position - transform->position;

	float forwardRad = atan2f(dif.x, dif.z);
	transform->rotation.y = forwardRad;
	m_pPhysics->velocity += transform->Forward() * -50.0f;	// ToDo:外部化
}

void CharaBase::setAnimationSpeed(const nlohmann::json& argument)
{
	m_Animator->SetPlaySpeed(argument.at("Speed").get<float>());
}
