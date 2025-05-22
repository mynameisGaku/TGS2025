#include "src/scene/play/chara/CharaBase.h"
#include "src/util/input/InputManager.h"
#include "src/util/fx/effect/EffectManager.h"

#include "src/common/component/physics/Physics.h"
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/stage/Stage.h"
#include "src/common/stage/StageObjectManager.h"
#include "src/scene/play/chara/CharaStamina.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/catcher/Catcher.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/common/component/animator/Animator.h"
#include "src/util/Utils.h"
#include "src/common/timeline/Timeline.h"
#include "src/util/file/json/VectorJson.h"

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
	m_SpeedScale			= 0.0f;
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
	m_Timeline				= nullptr;
	m_CanMove				= true;
	m_CanRot				= true;
	m_IsMove				= false;
	m_IsJumping				= false;

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

	m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
	m_FSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	Function::DeletePointer(m_FSM);
	Function::DeletePointer(m_SubFSM);
	Function::DeletePointer(m_Timeline);

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();

	Function::DeletePointer(m_EffectTransform);
}

void CharaBase::Init(std::string tag)
{
	m_CharaTag = tag;
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	m_Catcher->transform->scale = Vector3::Ones * CHARADEFINE_REF.CatchRadius * 2.0f;
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
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToJump", AnimOption().SetIsFixedRoot({ false, true, false }).SetOffset(Vector3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToRun", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "ActionIdleToStandingIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "AirSpin", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "CrouchToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "CrouchToRun", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "DamageToDown", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "Fall", AnimOption().SetIsLoop(true).SetIsFixedRoot({ false, true, false }).SetOffset(Vector3::SetY(100.0f)));
	m_Animator->LoadAnim("data/Animation/", "FallToCrouch", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "FallToRollToIdle", AnimOption().SetIsFixedRoot({ false, false, true }));

	m_Animator->LoadAnim("data/Animation/", "GetBall", AnimOption());

	m_Animator->LoadAnim("data/Animation/", "Run", AnimOption().SetIsLoop(true));
	m_Animator->LoadAnim("data/Animation/", "RunToActionIdle", AnimOption().SetIsFixedRoot({ false, false, true }));
	m_Animator->LoadAnim("data/Animation/", "RunToJump", AnimOption().SetIsFixedRoot({ false, true, true }).SetOffset(Vector3::SetY(100.0f)));
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

	// ボールの更新
	if (m_pBall)
	{
		m_ChargeRateWatchDog -= GTime.deltaTime;
		if (m_ChargeRateWatchDog < 0.0f)
		{
			m_ChargeRateWatchDog = 0.0f;
			m_IsCharging = false;
		}

		m_pBall->transform->position = VTransform(Vector3(0.0f, BALL_RADIUS, -BALL_RADIUS), MV1GetFrameLocalWorldMatrix(Model(), MV1SearchFrame(Model(), "mixamorig9:RightHand")));
	}

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
			if (toVec.GetLengthSquared() == 0)
				toVec = Vector3(0, 0, 1);

			// 反発力の強さを定義
			constexpr float REPELLENT_FORCE_SCALE_MAX = 2.0f;
			constexpr float REPELLENT_FORCE_SCALE_MIN = 1.0f;
			const float REPELLENT_RADIUS = collider->Radius() * 4.0f;

			// 反発力
			const Vector3 repellentForce = toVec.Normalize() * (REPELLENT_FORCE_SCALE_MIN + (REPELLENT_FORCE_SCALE_MAX - REPELLENT_FORCE_SCALE_MIN) * max(0.0f, 1.0f - toVec.GetLength() / REPELLENT_RADIUS));

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

		static const float DOWN_OFFSET = 10.0f;

		p1 = transform->Global().position + Vector3::SetY(37);
		p2 = capsuleCol->OffsetWorld() - Vector3::SetY(DOWN_OFFSET);

		const float radius = capsuleCol ? capsuleCol->Radius() : 20.0f;

		Vector3 pushVec;
		if (StageObjectManager::CollCheckCapsule(p1, p2, radius, &pushVec))
		{
			// 押し出しベクトルで位置を補正
			transform->position += pushVec;

			// Y成分が上方向なら接地とみなす
			if ((m_pPhysics->velocity.y <= 0.0f) and (pushVec.y > 0))
			{
				land();
				transform->position -= pushVec.Normalize() * DOWN_OFFSET;
				m_pPhysics->velocity.y = 0.0f;
				m_pPhysics->resistance.y = 0.0f;
			}
			else
			{
				m_IsLanding = false;

				// Y成分が下方向なら頭上ヒット（天井にぶつかった）
				if (pushVec.y < -0.1f)
				{
					m_pPhysics->velocity.y = min(m_pPhysics->velocity.y, 0.0f);
					m_IsJumping = false;
				}
			}
		}
		else
		{
			m_IsLanding = false;
		}

		// 衝突していなければ、通常の空中挙動へ
		if (not m_IsLanding)
		{
			m_pPhysics->SetGravity(GRAVITY);
			m_pPhysics->SetFriction(Vector3::Zero);
		}
	}
}

void CharaBase::Move(const Vector3& dir)
{
	m_IsMove = dir.GetLengthSquared() > 0;

	if (m_CanRot)
	{
		float currentRot = transform->rotation.y;	// 現在の向き
		float terminusRot = atan2f(dir.x, dir.z);		// 終点の向き

		// 徐々に終点の向きへ合わせる
		transform->rotation.y = Function::RotAngle(currentRot, terminusRot, m_RotSpeed);
	}

	if (m_CanMove)
	{
		const Vector3 normDir = Vector3::Normalize(dir);
		float deltaTimeMoveSpeed = m_MoveSpeed * m_SpeedScale * GTime.deltaTime;	// 時間経過率を適応した移動速度

		Vector3 velocity = normDir * deltaTimeMoveSpeed * Vector3::Horizontal;	// スティックの傾きの方向への速度

		// 速度を適応させる
		m_pPhysics->velocity.x = velocity.x;
		m_pPhysics->velocity.z = velocity.z;
	}
}

void CharaBase::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_IsJumping = true;
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

	m_SubFSM->ChangeState(&CharaBase::SubStateAimToThrow); // ステートを変更
}

void CharaBase::ThrowBallForward()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + Vector3::SetY(0.4f);

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_SubFSM->ChangeState(&CharaBase::SubStateAimToThrow); // ステートを変更
}

void CharaBase::ThrowHomingBall()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + Vector3::SetY(0.4f);
	//m_pBall->ThrowHoming(velocity * (1.0f + m_BallChargeRate), this);
	m_pBall->ThrowHoming(velocity * 30.0f, this);
	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_SubFSM->ChangeState(&CharaBase::SubStateAimToThrow); // ステートを変更
}

void CharaBase::GenerateBall()
{
	m_ChargeRateWatchDog = 0.1f;

	if (m_pBall != nullptr)
	{
		m_IsCharging = true;
		m_BallChargeRate += GTime.deltaTime;
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

	m_SubFSM->ChangeState(&CharaBase::SubStateGetBall); // ステートを変更
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

		m_EmoteTimer -= GTime.deltaTime;
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
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleEmote");
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
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleToJump");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		jumpUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Timeline->Stop();
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
		m_Timeline->Play("ActionIdleToStandingIdle");
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
		m_Timeline->Stop();
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
		m_Timeline->Play("Fall");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsLanding)
		{
			if (m_IsMove)
			{
				m_FSM->ChangeState(&CharaBase::StateFallToRoll); // ステートを変更
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
		m_Timeline->Stop();
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
		if (m_pPhysics->FlatVelocity().GetLengthSquared() > 0)
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

void CharaBase::StateFallToRoll(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("FallToRoll");
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
		m_Timeline->Stop();

		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

void CharaBase::StateRoll(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("Roll");
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
		m_Timeline->Stop();
		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

void CharaBase::StateRollToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		float frame = m_Animator->CurrentFrame();
		m_Timeline->Play("RollToActionIdle");
		m_Animator->SetCurrentFrame(frame);
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
		m_Timeline->Stop();
	}
	break;
	}
}

void CharaBase::StateRollToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		float frame = m_Animator->CurrentFrame();
		m_Timeline->Play("RollToRun");
		m_Animator->SetCurrentFrame(frame);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&CharaBase::StateRun); // ステートを変更

			m_Animator->Play("Run");

			static const float RUN_FROM_ROLL_START = 10.0f;
			m_Animator->SetCurrentFrame(RUN_FROM_ROLL_START);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Timeline->Stop();
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

void CharaBase::StateRunToActionIdle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("RunToActionIdle");
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
		m_Timeline->Stop();
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
		jumpUpdate();
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
		m_CanMove = false;
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
		m_CanMove = true;
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
		m_Timeline->Play("StandingIdleToActionIdle");
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
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_CatchTimer > 0.0f)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateCatch); // ステートを変更
		}
		else if (m_pBall != nullptr)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateHold); // ステートを変更
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

void CharaBase::SubStateGetBall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig9:Spine", "GetBall");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinishedSub("mixamorig9:Spine"))
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateHold); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	}
}

void CharaBase::SubStateHold(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig9:Spine", "Hold");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_pBall == nullptr)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	}
}

void CharaBase::SubStateHoldToAim(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig9:Spine", "HoldToAim");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinishedSub("mixamorig9:Spine"))
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateAimToThrow); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	}
}

void CharaBase::SubStateAimToThrow(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig9:Spine", "AimToThrow");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinishedSub("mixamorig9:Spine"))
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	}
}

void CharaBase::SubStateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		//テスト
		m_Animator->PlaySub("mixamorig9:Spine", "Catch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		catchUpdate();

		if (m_CatchTimer <= 0.0f)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		//テスト
		m_Animator->StopSub("mixamorig9:Spine");
	}
	break;
	}
}

//========================================================================

void CharaBase::land()
{
	m_IsLanding = true;
	m_IsJumping = false;
	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(FRICTION);
}

void CharaBase::idleUpdate()
{
	if (m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToJump); // ステートを変更
	}
	else if (not m_IsLanding)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
	}
	else if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateActionIdleToRun); // ステートを変更
	}
}

void CharaBase::runUpdate()
{
	if (m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToJump); // ステートを変更
	}
	else if (not m_IsLanding)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
	}
	else if (not m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRunToActionIdle); // ステートを変更
	}
	else if (m_SlideTimer > 0.0f)
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

	if (m_SlideTimer > 0.0f)
	{
		m_SlideTimer -= GTime.deltaTime;
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
}

void CharaBase::catchUpdate()
{
	if (m_CatchTimer > 0.0f)
	{
		m_CatchTimer -= GTime.deltaTime;
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

		m_pStamina->Use(CATCH_STAMINA_USE * GTime.deltaTime);
	}
	else
	{
		EffectManager::Stop("Catch_Ready_Single_Dust.efk", "Catch_Ready_Single_Dust" + m_CharaTag);
		EffectManager::Stop("Catch_Ready_Single_Tornado.efk", "Catch_Ready_Single_Tornado" + m_CharaTag);
	}
}

void CharaBase::jumpUpdate()
{
	if (m_Animator->IsFinished())
	{
		m_IsJumping = false;
	}

	if (not m_IsJumping)
	{
		m_FSM->ChangeState(&CharaBase::StateFall); // ステートを変更
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
	m_FSM->ChangeState(&CharaBase::StateRoll); // ステートを変更
}

void CharaBase::endRoll(const nlohmann::json& argument)
{
	if (m_IsMove)
	{
		m_FSM->ChangeState(&CharaBase::StateRollToRun); // ステートを変更
	}
	else
	{
		m_FSM->ChangeState(&CharaBase::StateRollToActionIdle); // ステートを変更
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
