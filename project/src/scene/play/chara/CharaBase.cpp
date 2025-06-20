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
#include "src/common/timeline/Timeline.h"
#include "src/util/file/json/VectorJson.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/ptr/PtrUtil.h"
#include "src/util/math/MathUtil.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include <src/util/math/Vector3Util.h>
#include "src/common/component/model_frame_trail_renderer/ModelFrameTrailRenderer.h"
#include "src/common/component/model_frame_trail_renderer/MODEL_FRAME_TRAIL_RENDERER_DESC.h"
#include "src/util/math/Random.h"
#include "src/util/sound/SoundManager.h"
#include "src/scene/play/tackler/Tackler.h"

#include "src/util/ui/UI_Manager.h"
#include "src/util/ui/UI_Gauge.h"
#include "src/scene/play/ui/UI_CrossHair.h"
#include "src/scene/play/ui/UI_HitPoint_Icon.h"
#include <src/reference/ball/BallRef.h>

using namespace KeyDefine;

namespace
{
	static const float CATCH_STAMINA_USE = 50.0f;	// キャッチに使うスタミナ（毎秒）
	static const float CATCH_STAMINA_MIN = 0.0f;	// キャッチを開始するのに必要な残スタミナ
	static const float CATCH_TIME = 0.05f;	// 入力一回のキャッチ継続時間
	static const float SLIDE_TIME = 0.05f;	// 入力一回のスライディング継続時間
	static const float CHARGE_TIME = 1.0f;
	static const float CHARGE_BALLSPEED = 1.5f;
}

CharaBase::CharaBase()
{
	m_pStamina			= Instantiate<CharaStamina>();
	m_pHP				= Instantiate<CharaHP>();
	m_pBall				= nullptr;
	m_pLastBall			= nullptr;
	m_pPhysics			= nullptr;
	m_BallChargeRate	= 0.0f;
	m_IsCharging		= false;
	m_MoveSpeed			= 0.0f;
	m_RotSpeed			= 0.0f;
	m_SpeedScale		= 0.0f;
	m_CharaTag			= CHARADEFINE_REF.Tags[0];
	m_Catcher			= nullptr;
	m_Tackler			= nullptr;
	m_Index				= 0;
	m_Animator			= nullptr;
	m_EmoteTimer		= 0.0f;
	m_IsLanding			= true;
	m_SlideTimer		= 0.0f;
	m_EffectTransform	= nullptr;
	m_pBallManager		= nullptr;
	m_Timeline			= nullptr;
	m_CanMove			= true;
	m_CanRot			= true;
	m_IsMove			= false;
	m_IsJumping			= false;
	m_CanCatch			= true;
	m_CanThrow			= true;
	m_IsCatching		= false;
	m_CanHold			= true;
	m_CanTackle			= true;
	m_IsInvincible		= false;
	m_pHitBall			= nullptr;
	m_pStatusTracker	= nullptr;
	m_pCatchReadyEffect	= nullptr;
	m_pCatchDustEffect	= nullptr;
	m_CatchTimer		= 0.0f;

	m_UI_CrossHair = nullptr;
	m_UI_BallChargeMeter = nullptr;
	m_UI_HitPointIcon = nullptr;

	m_HitPoint = 0;
	m_Stamina = 0.0f;

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
	m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
}

CharaBase::~CharaBase()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	PtrUtil::SafeDelete(m_FSM);
	PtrUtil::SafeDelete(m_SubFSM);
	PtrUtil::SafeDelete(m_Timeline);

	PtrUtil::SafeDelete(m_UI_CrossHair);
	PtrUtil::SafeDelete(m_UI_BallChargeMeter);
	PtrUtil::SafeDelete(m_UI_HitPointIcon);

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();

	/*for (int i = 0; i < 5; ++i)
	{
		PtrUtil::SafeDelete(m_pTrail[i]);
	}*/

	PtrUtil::SafeDelete(m_EffectTransform);
}

void CharaBase::Init(std::string tag)
{
	m_Alarm = new Alarm;
	m_Alarm->Reset();

	m_CharaTag = tag;
	m_pStatusTracker = new StatusTracker();
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	m_Catcher->transform->scale = Vector3::Ones * CHARADEFINE_REF.CatchRadius * 2.0f;
	m_Catcher->transform->SetParent(transform);
	m_Catcher->Init(tag);
	m_Catcher->SetColliderActive(false);
	m_Catcher->SetParent(this);

	m_Tackler = Instantiate<Tackler>();
	m_Tackler->transform->position = Vector3(0.0f, CHARADEFINE_REF.TackleRadius, CHARADEFINE_REF.TackleRadius);
	m_Tackler->transform->scale = Vector3::Ones * CHARADEFINE_REF.TackleRadius * 2.0f;
	m_Tackler->transform->SetParent(transform);
	m_Tackler->Init(tag);
	m_Tackler->SetColliderActive(false);
	m_Tackler->SetParent(this);

	m_UI_CrossHair = new UI_CrossHair(RectTransform(), m_Index);
	m_UI_CrossHair->SetScroll(&m_Stamina, 0.0f, m_pStamina->GetMax(), Gauge::ScrollType::eUp, false);
	m_UI_CrossHair->SetHandle_CrossHair				("data/texture/UI/CrossHair/CrossHair.png");
	m_UI_CrossHair->SetHandle_CrossHairFrame			("data/texture/UI/CrossHair/CrossHairFrame.png");
	m_UI_CrossHair->SetHandle_CrossHairOutSide		("data/texture/UI/CrossHair/CrossHairOutSide.png");
	m_UI_CrossHair->SetHandle_CrossHairOutSideBack	("data/texture/UI/CrossHair/CrossHairOutSideBack.png");

	m_UI_BallChargeMeter = new UI_CrossHair(RectTransform(), m_Index);
	m_UI_BallChargeMeter->SetScroll(&m_BallChargeRate, 0.0f, 1.0f, Gauge::ScrollType::eUp, false);
	m_UI_BallChargeMeter->SetHandle_CrossHairOutSide("data/texture/UI/CrossHair/BallChargeMeter_00.png");

	m_UI_HitPointIcon = new UI_HitPoint_Icon(RectTransform(), m_Index);
	m_UI_HitPointIcon->SetValue(&m_HitPoint, 0.0f, m_pHP->GetMax(), m_pHP->GetMax());
	m_UI_HitPointIcon->SetImage(LoadGraph("data/texture/ui/HP/HitPoint.png"));

	std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC> descs;
	std::vector<std::pair<std::string, std::string>> frameAndTrailNames = {
		{ "mixamorig9:Hips", "HipsTrail" },
		{ "mixamorig9:Spine2", "Spine1Trail" },
		{ "mixamorig9:LeftShoulder", "LeftShoulderTrail" },
		{ "mixamorig9:RightShoulder", "RightShoulderTrail" },
		{ "mixamorig9:LeftLeg", "LeftLegTrail" },
		{ "mixamorig9:LeftUpLeg", "LeftUpLegTrail" },
		{ "mixamorig9:RightLeg", "RightLegTrail" },
		{ "mixamorig9:RightUpLeg", "RightUpLegTrail"}
	};
	ModelFrameTrailRenderer* trail = AddComponent<ModelFrameTrailRenderer>();
	MODEL_FRAME_TRAIL_RENDERER_DESC desc1;
	MODEL_FRAME_TRAIL_RENDERER_DESC desc2;

	auto addTrail = []()
		{
			float lt = 0.4f; // トレイルの寿命
			float lifeTime = lt * Random.GetFloatRange(0.8f, 1.5f);

			MODEL_FRAME_TRAIL_RENDERER_DESC descBold{};
			descBold.interval = 1; // フレーム間隔（何フレームごとに描画するか）
			descBold.subdivisions = 16; // 補間分割数（大きいほど滑らか）
			descBold.thick = 50.0f; // トレイルの太さ
			descBold.lifeTime = lifeTime; // トレイルの寿命
			descBold.appearRate = 0.5f; // トレイルが出現する確率（0.0f～1.0f）
			descBold.posRandomRange = Vector3(3.0f, 3.0f, 3.0f);

			MODEL_FRAME_TRAIL_RENDERER_DESC descSmall{};
			descSmall.interval = 1; // フレーム間隔（何フレームごとに描画するか）
			descSmall.subdivisions = 16; // 補間分割数（大きいほど滑らか）
			descSmall.thick = 25.0f; // トレイルの太さ
			descSmall.lifeTime = lifeTime; // トレイルの寿命
			descSmall.appearRate = 0.5f; // トレイルが出現する確率（0.0f～1.0f）
			descSmall.posRandomRange = Vector3(3.0f, 3.0f, 3.0f);

			return std::pair<MODEL_FRAME_TRAIL_RENDERER_DESC, MODEL_FRAME_TRAIL_RENDERER_DESC>(descBold, descSmall);
		};
	descs.reserve(frameAndTrailNames.size());
	for (const auto& pair : frameAndTrailNames)
	{
		desc1 = addTrail().first;
		desc1.frameName = pair.first;
		desc1.trailName = pair.second;
		desc2 = addTrail().second;
		desc2.frameName = pair.first;
		desc2.trailName = pair.second;

		descs.push_back(desc1);
		descs.push_back(desc2);
	}
	trail->Finalize(Model(), descs, m_hTrailImage);

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 160.0f;
	m_EffectTransform->position.z = -100.0f;
	m_EffectTransform->rotation.y = MathUtil::ToRadians(180.0f);

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
	m_Timeline->SetFunction("ThrowBall", &CharaBase::throwBall);
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
		MATRIX m = MV1GetFrameLocalWorldMatrix(Model(), MV1SearchFrame(Model(), "mixamorig9:RightHandThumb1"));
		Vector3 dir = Vector3(0, 0, 1) * MGetRotElem(m);

		m_pBall->transform->position = Vector3(0.0f, BALL_RADIUS, -BALL_RADIUS);
		m_pBall->transform->position *= m;
		m_pBall->transform->rotation = Vector3Util::DirToEuler(dir);
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
	m_IsCatching = false;

	m_Stamina = m_pStamina->GetCurrent();
	m_HitPoint = m_pHP->GetCurrent();

	Object3D::Update();

	invincibleUpdate();
}

void CharaBase::Draw()
{
	Object3D::Draw();

	/*for (int i = 0; i < 5; i++)
	{
		m_pTrail[i]->Draw();
	}*/

	if (m_pHP->IsDead())
	{
		DrawFormatString(300, 300 + m_Index * 40, 0xff0000, std::string("Dead [index:" + std::to_string(m_Index) + "]").c_str());
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
			if (ball->GetState() == Ball::S_LANDED)
				return;

			if (m_IsInvincible)
			{
				EffectManager::Play3D("Hit_Wall.efk", *transform->Copy(), "Hit_Wall" + m_CharaTag);
				return;
			}

			if (m_CharaTag == "Blue")
			{
				EffectManager::Play3D("Hit_Blue.efk", *transform->Copy(), "Hit_Blue" + m_CharaTag);
			}
			else
			{
				EffectManager::Play3D("Hit_Red.efk", *transform->Copy(), "Hit_Red" + m_CharaTag);
			}

			getHit(ball);
			m_pHP->Damage_UseDefault();
			m_pStatusTracker->AddGetHitCount(1);

			ball->GetLastOwner()->GetStatusTracker()->AddHitCount(1);
			if (m_pHP->IsDead())
			{
				m_pStatusTracker->AddDeathCount(1);
				ball->GetLastOwner()->GetStatusTracker()->AddKillCount(1);
				Respawn(Vector3::Zero, Vector3::Zero);
			}
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
		transform->rotation.y = MathUtil::RotAngle(currentRot, terminusRot, m_RotSpeed);
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

void CharaBase::GenerateBall()
{
	if (not m_CanCatch) return;

	if (m_pBall != nullptr)
		return;

	if (m_pBallManager == nullptr)
		m_pBallManager = FindGameObject<BallManager>();

	SetBall(m_pBallManager->CreateBall(transform->Global().position));
}

void CharaBase::SetBall(Ball* ball)
{
	m_pBall = ball;

	if (m_pBall == nullptr)
		return;

	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	m_pBall->Reset(m_CharaTag);

	m_IsCharging = false;
}

void CharaBase::StartBallCharge()
{
	m_IsCharging = true;
	m_SubFSM->ChangeState(&CharaBase::SubStateHoldToAim); // ステートを変更
}

void CharaBase::StartThrow()
{
	if (m_pBall == nullptr)
		return;
	if (not m_CanThrow)
		return;

	m_FSM->ChangeState(&CharaBase::StateAimToThrow); // ステートを変更
}

void CharaBase::ThrowBall(const Vector3& velocity)
{
	if (m_pBall == nullptr)
		return;

	m_pBall->Throw(velocity * (1.0f + m_BallChargeRate * CHARGE_BALLSPEED), this);

	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
}

void CharaBase::ThrowBallForward()
{
	Vector3 forward = transform->Forward();
	Vector3 velocity = forward + Vector3::SetY(0.15f);	// Magic:)

	ThrowBall(velocity);
}

void CharaBase::ThrowHomingBall()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 velocity = (forward * 35.0f) + Vector3::SetY(0.3f);	// Magic:)
	
	const CharaBase* targetChara = nullptr;
	Camera* camera = CameraManager::GetCamera(m_Index);

	if (camera != nullptr)
		targetChara = camera->TargetChara();	// カメラのターゲットキャラを取得

	m_pBall->ThrowHoming(velocity * (1.0f + m_BallChargeRate * CHARGE_BALLSPEED), this, targetChara, 0.1f + m_BallChargeRate);
	m_pStatusTracker->AddThrowCount(1);
	m_pLastBall = m_pBall;
	m_pBall = nullptr;

	playThrowSound();

	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
}

void CharaBase::Feint()
{
	if (not m_CanThrow)
		return;
	m_FSM->ChangeState(&CharaBase::StateFeint); // ステートを変更
}

void CharaBase::TeleportToLastBall()
{
	if (m_pLastBall == nullptr) return;
	transform->position = m_pLastBall->transform->position;

	// ToDo:消える演出
	m_pLastBall->SetIsActive(false);

	GenerateBall();

	m_pLastBall = nullptr;
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_FSM->ChangeState(&CharaBase::StateAirSpin);
}

void CharaBase::DropBall(const Vector3& other, float force_vertical, float force_horizontal)
{
	if (not m_pBall)
		return;

	m_pBall->ChangeState(Ball::State::S_LANDED);
	m_pBall->GetComponent<Physics>()->SetGravity(GRAVITY);
	m_pBall->Knockback(other, force_vertical, force_horizontal);
	m_pBall->SetOwner(nullptr);
	m_pBall = nullptr;
}

void CharaBase::Catch()
{
	if (not m_CanCatch) return;

	if (m_pStamina->GetCurrent() > CATCH_STAMINA_MIN)
	{
		m_IsCatching = true;
	}
}

void CharaBase::Respawn(const Vector3& pos, const Vector3& rot)
{
	m_pHP->Reset();
	m_pStamina->Reset();
	m_pBall = nullptr;
	m_pLastBall = nullptr;
	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
	m_IsJumping = false;
	m_IsLanding = true;
	transform->position = pos;
	transform->rotation = rot;
	// キャッチャーの位置を更新
	if (m_Catcher)
	{
		m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	}
}

void CharaBase::SetTrailImage(int hImage)
{
	m_hTrailImage = hImage;
}

Vector2 CharaBase::Target(const Ball* ball)
{
	return Vector2();
}

void CharaBase::CatchSuccess(const Vector3& velocity)
{
	m_CanCatch = false;
	m_CanMove = false;
	m_CanRot = false;
	m_Catcher->SetColliderActive(false);

	playCatchBallSound();

	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(Vector3::Zero);

	transform->rotation.y = atan2f(transform->position.x - velocity.x, transform->position.z - velocity.z);
	m_pPhysics->velocity.y = 10.0f;
	m_pPhysics->velocity += Vector3::Normalize(velocity) * Vector3(1, 0, 1) * 10.0f;

	m_SubFSM->ChangeState(&CharaBase::SubStateGetBall); // ステートを変更
}

void CharaBase::Tackle()
{
	if (not m_CanTackle)
		return;

	m_IsTackling = true;
}

void CharaBase::GetTackle(const Vector3& other, float force_horizontal, float force_vertical, bool isForceKnockback)
{
	if (m_IsInvincible && not isForceKnockback)
	{
		return;
	}

	Knockback(other, force_vertical, force_horizontal);

	SetInvincible(CHARADEFINE_REF.TackleInvincibleDurationSec, true);

	DropBall(transform->position, BALL_REF.DropForce_Vertical, BALL_REF.DropForce_Horizontal);

	m_FSM->ChangeState(&CharaBase::StateDamageToDown);
}

void CharaBase::SetInvincible(float duration_sec, bool isOverride)
{
	if (!isOverride 
		&& m_InvincibleTimer > 0.0f
		)
	{
		return;
	}

	m_InvincibleTimer = duration_sec;
	m_IsInvincible = true;
}

void CharaBase::invincibleUpdate()
{
	if (m_InvincibleTimer > 0.0f)
	{
		m_InvincibleTimer -= GTime.deltaTime;
		m_IsInvincible = true;
	}
	else
	{
		m_IsInvincible = false;
		m_InvincibleTimer = 0.0f;
	}
}

void CharaBase::Knockback(const Vector3& other, float force_vertical, float force_horizontal)
{
	Vector3 otherToMe = transform->position - other;
	otherToMe.y += 20.0f;
	Vector3 impactNorm = otherToMe.Normalize();
	Vector3 impactVertical = Vector3(0.0f, impactNorm.y, 0.0f) * force_vertical;
	Vector3 impactHorizontal = Vector3(impactNorm.x, 0.0f, impactNorm.z) * force_horizontal;
	Vector3 impact = impactVertical + impactHorizontal;

	m_pPhysics->velocity += impact;
}

//========================================================================
// メインステート

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

void CharaBase::StateActionIdleToTackle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("ActionIdleToTackle");
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

void CharaBase::StateAimToThrow(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		m_Timeline->Play("AimToThrow");
		m_CanCatch = false;
		m_CanHold = false;
		m_CanThrow = false;
		m_CanTackle = false;
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
		m_CanCatch = true;
		m_CanHold = true;
		m_CanThrow = true;
		m_CanTackle = true;
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

		m_CanCatch	= false;
		m_CanMove	= false;
		m_CanRot	= false;
		m_CanTackle = false;
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
		m_CanCatch	= true;
		m_CanMove	= true;
		m_CanRot	= true;
		m_CanTackle = true;
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
		m_CanTackle = true;
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
		if (m_IsTackling)
		{
			m_FSM->ChangeState(&CharaBase::StateTackle);
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
		m_CanTackle = false;
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
		m_CanTackle = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsTackling)
		{
			m_FSM->ChangeState(&CharaBase::StateTackle);
		}

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
		m_CanTackle = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsTackling)
		{
			m_FSM->ChangeState(&CharaBase::StateTackle);
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

		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

void CharaBase::StateFeint(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		m_Timeline->Play("AimToThrow");
		m_CanCatch = false;
		m_CanHold = false;
		m_CanThrow = false;
		m_CanTackle = false;
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
		m_SubFSM->ChangeState(&CharaBase::SubStateHoldToAim); // ステートを変更

		m_CanCatch = true;
		m_CanHold = true;
		m_CanThrow = true;
		m_CanTackle = true;

		m_Timeline->Stop();
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

void CharaBase::StateRunToTackle(FSMSignal sig)
{
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

void CharaBase::StateTackle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("Tackle");

		m_CanMove = false;
		m_CanRot = false;

		m_Tackler->SetColliderActive(true);

		SetInvincible(CHARADEFINE_REF.TackleInvincibleDurationSec, true);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		tackleUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
		if (m_Animator->IsFinished())
		{
			if (m_Alarm->IsFinish())
				m_FSM->ChangeState(&CharaBase::StateActionIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Timeline->Stop();
		m_Tackler->SetColliderActive(false);
		m_IsTackling = false;
		m_CanMove = true;
		m_CanRot = true;
	}
	break;
	}
}

//========================================================================
// サブステート

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
		if (m_CanCatch && m_IsCatching)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateCatch); // ステートを変更
		}
		else if (m_CanHold && m_pBall != nullptr)
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
		m_CanMove = false;
		m_CanRot = false;
		m_pPhysics->SetGravity(Vector3::Zero);
		m_pPhysics->SetFriction(Vector3::Zero);
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
		m_CanMove = true;
		m_CanRot = true;
		m_pPhysics->SetGravity(GRAVITY);
		m_pPhysics->SetGravity(FRICTION);
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
		m_BallChargeRate += GTime.deltaTime / CHARGE_TIME;
		if (m_BallChargeRate > 1.0f)
		{
			m_BallChargeRate = 1.0f;
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

void CharaBase::SubStateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig9:Spine", "Catch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (not m_IsCatching)
		{
			m_SubFSM->ChangeState(&CharaBase::SubStateNone); // ステートを変更
		}

		catchUpdate();
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_Catcher->SetColliderActive(false);

		if (m_CharaTag == "Blue")
		{
			EffectManager::Stop("Catch_Dust_Blue.efk", "Catch_Dust_Blue" + m_CharaTag);
			EffectManager::Stop("Catch_Ready_Blue.efk", "Catch_Ready_Blue" + m_CharaTag);
		}
		else if (m_CharaTag == "Red")
		{
			EffectManager::Stop("Catch_Dust_Red.efk", "Catch_Dust_Red" + m_CharaTag);
			EffectManager::Stop("Catch_Ready_Red.efk", "Catch_Ready_Red" + m_CharaTag);
		}
		//m_pCatchReadyEffect = nullptr;
		//m_pCatchDustEffect = nullptr;
	}
	break;
	}
}

//========================================================================
// private関数

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
	else if (m_IsTackling)
	{
		m_FSM->ChangeState(&CharaBase::StateTackle);
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
	else if (m_IsTackling)
	{
		m_FSM->ChangeState(&CharaBase::StateTackle);
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

	if (m_IsTackling)
	{
		m_FSM->ChangeState(&CharaBase::StateTackle);
	}
}

void CharaBase::catchUpdate()
{
	if (m_IsCatching)
	{
		//if (not m_Catcher->IsColliderActive())
		//{
			if (m_EffectTransform != nullptr)
			{
				if (m_CharaTag == "Blue")
				{
					//m_pCatchDustEffect = EffectManager::Play3D_Loop("Catch_Dust_Blue.efk", m_EffectTransform->Global(), "Catch_Dust_Blue" + m_CharaTag);
					//m_pCatchReadyEffect = EffectManager::Play3D_Loop("Catch_Ready_Blue.efk", m_EffectTransform->Global(), "Catch_Ready_Blue" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Dust_Blue.efk", m_EffectTransform->Global(), "Catch_Dust_Blue" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Ready_Blue.efk", m_EffectTransform->Global(), "Catch_Ready_Blue" + m_CharaTag);
				}
				else if(m_CharaTag == "Red")
				{
					//m_pCatchDustEffect = EffectManager::Play3D_Loop("Catch_Dust_Red.efk", m_EffectTransform->Global(), "Catch_Dust_Red" + m_CharaTag);
					//m_pCatchReadyEffect = EffectManager::Play3D_Loop("Catch_Ready_Red.efk", m_EffectTransform->Global(), "Catch_Ready_Red" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Dust_Red.efk", m_EffectTransform->Global(), "Catch_Dust_Red" + m_CharaTag);
					EffectManager::Play3D_Loop("Catch_Ready_Red.efk", m_EffectTransform->Global(), "Catch_Ready_Red" + m_CharaTag);
				}
			}
		//}

		m_Catcher->SetColliderActive(true);
		m_pStamina->Use(CATCH_STAMINA_USE * GTime.deltaTime);

		// カメラの向きに合わせる
		m_CanRot = false;

		Camera* camera = CameraManager::GetCamera(m_Index);
		if (camera != nullptr) {
			float currentY = transform->rotation.y;
			float terminusY = camera->transform->rotation.y;
			transform->rotation.y = MathUtil::LerpAngle(currentY, terminusY, 0.5f);
		}
	}
	else
	{
		m_Catcher->SetColliderActive(false);
		m_CanRot = true;
		//m_pCatchReadyEffect = nullptr;
		//m_pCatchDustEffect = nullptr;
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

	if (m_IsTackling)
	{
		m_FSM->ChangeState(&CharaBase::StateTackle);
	}
}

void CharaBase::tackleUpdate()
{
	if (m_Animator->IsFinished())
	{
		// あと隙
		m_Alarm->Set(0.1f); // magic:>
	}
}

void CharaBase::getHit(Ball* hit)
{
	m_FSM->ChangeState(&CharaBase::StateDamageToDown);

	Vector3 dif = hit->transform->position - transform->position;

	m_pHitBall = hit;

	float forwardRad = atan2f(dif.x, dif.z);
	transform->rotation.y = forwardRad;
	m_pPhysics->velocity += transform->Forward() * -50.0f;	// ToDo:外部化

	playGetHitSound();
}

void CharaBase::playThrowSound()
{
	int rand = Random.GetIntRange(0, 100);

    std::string base = "SE_throw_";
	std::string power = "";
	std::string num = "00";
	std::string fileType = ".mp3";

    if (m_BallChargeRate < 0.2f)
    {
        power += "weak_";
    }
    else if (m_BallChargeRate < 0.8f)
    {
        power += "normal_";
    }
    else
    {
        power += "strong_";
    }

    if (rand < 50)
    {
        num = "01";
    }

    std::string soundName = base + power + num + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void CharaBase::playGetHitSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_hit_";
	std::string num = "00";
	std::string fileType = ".mp3";

	if (rand < 50)
	{
		num = "01";
	}

	std::string soundName = base + num + fileType;


	std::string base2 = "SE_bound_ball";
	std::string num2 = "";

	if (rand < 33)
	{
		num = "00";
	}
	else if (rand < 66)
	{
		num = "01";
	}
	else
	{
		num = "02";
	}
	std::string soundName2 = base2 + num2 + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);

}

void CharaBase::playFootStepSound()
{
}

void CharaBase::playCatchBallSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_bound_ball";
	std::string num = "";
	std::string fileType = ".mp3";

	if (rand < 33)
	{
		num = "00";
	}
    else if (rand < 66)
    {
        num = "01";
    }
    else
    {
        num = "02";
    }

	std::string base2 = "SE_catch_success_";
	std::string num2 = "";
    if (rand < 50)
    {
        num2 = "00";
    }
    else
    {
        num2 = "01";
    }

	std::string soundName = base + num + fileType;
    std::string soundName2 = base2 + num2 + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
	if (not SoundManager::IsPlaying(soundName2, soundName2))
		SoundManager::Play(soundName2, soundName2);
}

void CharaBase::playPickupBallSound()
{
}

void CharaBase::playVacuumSound()
{
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

void CharaBase::throwBall(const nlohmann::json& argument)
{
	if (m_FSM->GetCurrentState() == &CharaBase::StateAimToThrow)
	{
		ThrowHomingBall();
	}
}
