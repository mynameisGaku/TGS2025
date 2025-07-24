#include "src/scene/play/chara/Chara.h"
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
#include "src/scene/play/chara/CharaSpawnPointManager.h"
#include "src/scene/play/ball/BallTarget.h"

#include "src/util/ui/UI_Manager.h"
#include "src/util/ui/UI_Gauge.h"
#include "src/scene/play/ui/UI_CrossHair.h"
#include "src/scene/play/ui/UI_HitPoint_Icon.h"

#include <src/reference/ball/BallRef.h>

#include "src/scene/play/chara/CharaSpawnPointManager.h"
#include "src/scene/play/ui/UI_Fade.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>

using namespace KeyDefine;

namespace
{
	static const float CATCH_STAMINA_USE = 50.0f;	// キャッチに使うスタミナ（毎秒）
	static const float CATCH_STAMINA_MIN = 0.0f;	// キャッチを開始するのに必要な残スタミナ
	static const float CATCH_TIME = 0.05f;	// 入力一回のキャッチ継続時間
	static const float SLIDE_TIME = 0.05f;	// 入力一回のスライディング継続時間
	static const float CHARGE_TIME = 1.0f;
	static const float CHARGE_BALLSPEED = 1.5f;
	static const Vector3 CHARA_GRAVITY = GRAVITY * 3.0f;
	static const float ARM_HEIGHT = 100.0f;	// 腕の高さ
}

Chara::Chara()
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
	m_CanClimb			= true;
	m_IsClimb			= false;
	m_IsWall			= false;
	m_pHitBall			= nullptr;
	m_pStatusTracker	= nullptr;
	m_pCatchReadyEffect	= nullptr;
	m_pCatchDustEffect	= nullptr;
	m_CatchTimer		= 0.0f;
	m_hTrailImage		= -1;
	m_IsTargeting		= false;
	m_IsTargeted		= false;
	m_pTargetBall		= nullptr;
	m_IsInhibitionSpeed = true;
	m_SpawnPointManager = nullptr;
	m_TackleIntervalAlarm = nullptr;
	m_BallTarget = nullptr;

	m_HitPoint = 0;
	m_Stamina = 0.0f;

	m_Alarm = nullptr;
	m_lastUpdatePosition = Vector3::Zero;
	m_InvincibleTimer = 0.0f;
	m_IsDamage = false;
	m_IsTackling = false;

	m_FSM = new TinyFSM<Chara>(this);
	m_SubFSM = new TinyFSM<Chara>(this);
	m_RespawnFSM = new TinyFSM<Chara>(this);

	m_pCharaSpawnPointManager = nullptr;

	m_pUI_ButtonHint = nullptr;
	m_pUI_Fade = nullptr;
#if TRUE
	// この行程はデバッグ用。関数ポインタはコンパイル後に関数名が保持されないので、プロファイリングするにはこの行程が必須。
	m_FSM->RegisterStateName(&Chara::StateActionIdle, "StateActionIdle");
	m_FSM->RegisterStateName(&Chara::StateActionIdleEmote, "StateActionIdleEmote");
	m_FSM->RegisterStateName(&Chara::StateActionIdleToJump, "StateActionIdleToJump");
	m_FSM->RegisterStateName(&Chara::StateActionIdleToRun, "StateActionIdleToRun");
	m_FSM->RegisterStateName(&Chara::StateActionIdleToStandingIdle, "StateActionIdleToStandingIdle");
	m_FSM->RegisterStateName(&Chara::StateActionIdleToTackle, "StateActionIdleToTackle");
	m_FSM->RegisterStateName(&Chara::StateAimToThrow, "StateAimToThrow");
	m_FSM->RegisterStateName(&Chara::StateAirSpin, "StateAirSpin");
	m_FSM->RegisterStateName(&Chara::StateCrouchToActionIdle, "StateCrouchToActionIdle");
	m_FSM->RegisterStateName(&Chara::StateCrouchToRun, "StateCrouchToRun");
	m_FSM->RegisterStateName(&Chara::StateDamageToDown, "StateDamageToDown");
	m_FSM->RegisterStateName(&Chara::StateFall, "StateFall");
	m_FSM->RegisterStateName(&Chara::StateFallToCrouch, "StateFallToCrouch");
	m_FSM->RegisterStateName(&Chara::StateFallToRoll, "StateFallToRoll");
	m_FSM->RegisterStateName(&Chara::StateFeint, "StateFeint");
	m_FSM->RegisterStateName(&Chara::StateRoll, "StateRoll");
	m_FSM->RegisterStateName(&Chara::StateRollToActionIdle, "StateRollToActionIdle");
	m_FSM->RegisterStateName(&Chara::StateRollToRun, "StateRollToRun");
	m_FSM->RegisterStateName(&Chara::StateRun, "StateRun");
	m_FSM->RegisterStateName(&Chara::StateRunToActionIdle, "StateRunToActionIdle");
	m_FSM->RegisterStateName(&Chara::StateRunToJump, "StateRunToJump");
	m_FSM->RegisterStateName(&Chara::StateRunToSlide, "StateRunToSlide");
	m_FSM->RegisterStateName(&Chara::StateRunToTackle, "StateRunToTackle");
	m_FSM->RegisterStateName(&Chara::StateSlide, "StateSlide");
	m_FSM->RegisterStateName(&Chara::StateSlideToRun, "StateSlideToRun");
	m_FSM->RegisterStateName(&Chara::StateStandingIdle, "StateStandingIdle");
	m_FSM->RegisterStateName(&Chara::StateStandingIdleEmote, "StateStandingIdleEmote");
	m_FSM->RegisterStateName(&Chara::StateStandingIdleToActionIdle, "StateStandingIdleToActionIdle");
	m_FSM->RegisterStateName(&Chara::StateTackle, "StateTackle");
	m_FSM->RegisterStateName(&Chara::StateWallStepLeft, "StateWallStepLeft");
	m_FSM->RegisterStateName(&Chara::StateWallStepRight, "StateWallStepRight");

	m_SubFSM->RegisterStateName(&Chara::SubStateCatch, "SubStateCatch");
	m_SubFSM->RegisterStateName(&Chara::SubStateGetBall, "SubStateGetBall");
	m_SubFSM->RegisterStateName(&Chara::SubStateHold, "SubStateHold");
	m_SubFSM->RegisterStateName(&Chara::SubStateHoldToAim, "SubStateHoldToAim");
	m_SubFSM->RegisterStateName(&Chara::SubStateNone, "SubStateNone");

	m_RespawnFSM->RegisterStateName(&Chara::RespawnStateFadeIn, "RespawnStateFadeIn");
	m_RespawnFSM->RegisterStateName(&Chara::RespawnStateFadeOut, "RespawnStateFadeOut");
	m_RespawnFSM->RegisterStateName(&Chara::RespawnStateNone, "RespawnStateNone");
#endif // FALSE

	main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
	sub_changeStateNetwork(&Chara::SubStateNone); // ステートを変更
}

Chara::~Chara()
{
	std::string output = "cha delete : " + std::to_string(m_Index) + "\n";
	OutputDebugString(output.c_str());

	PtrUtil::SafeDelete(m_FSM);
	PtrUtil::SafeDelete(m_SubFSM);
	PtrUtil::SafeDelete(m_Timeline);

	m_Catcher->SetParent(nullptr);
	m_Catcher->DestroyMe();

	/*for (int i = 0; i < 5; ++i)
	{
		PtrUtil::SafeDelete(m_pTrail[i]);
	}*/

	PtrUtil::SafeDelete(m_EffectTransform);

	Camera* camera = CameraManager::GetCamera(m_Index);
	if (camera != nullptr)
		camera->SetFollowerChara(nullptr);
}

void Chara::Init(std::string tag)
{
	m_Alarm = new Alarm;
	m_Alarm->Reset();

	m_lastUpdatePosition = Vector3::Zero;

	m_CharaTag = tag;
	m_pStatusTracker = new StatusTracker();
	m_pPhysics = GetComponent<Physics>();
	m_Catcher = Instantiate<Catcher>();
	m_Catcher->transform->position = Vector3(0.0f, ARM_HEIGHT, 0.0f);
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

	m_SpawnPointManager = FindGameObject<CharaSpawnPointManager>();

	std::string sIndex;
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		sIndex = "1P";
	}
	else
	{
		sIndex = std::to_string(m_Index + 1) + "P";
	}

	UI_CrossHair* ui_CrossHair = UI_Manager::Find<UI_CrossHair>("CrossHair_" + sIndex);
	if (ui_CrossHair != nullptr)
	{
		ui_CrossHair->SetHandle_CrossHair("data/texture/UI/CrossHair/CrossHair.png");
		ui_CrossHair->SetHandle_CrossHairFrame("data/texture/UI/CrossHair/CrossHairFrame.png");
		ui_CrossHair->SetHandle_CrossHairOutSide("data/texture/UI/CrossHair/CrossHairOutSide.png");
		ui_CrossHair->SetHandle_CrossHairOutSideBack("data/texture/UI/CrossHair/CrossHairOutSideBack.png");
		ui_CrossHair->SetScroll(&m_Stamina, 0.0f, m_pStamina->GetMax(), Gauge::ScrollType::eUp, false);
	}

	UI_HitPoint_Icon* ui_HitPoint_Icon = UI_Manager::Find<UI_HitPoint_Icon>("HitPoint_Icon_" + sIndex);
	if (ui_HitPoint_Icon != nullptr)
	{
		ui_HitPoint_Icon->SetValue(&m_HitPoint, 0.0f, m_pHP->GetMax(), m_pHP->GetMax());
		ui_HitPoint_Icon->SetImage(LoadGraph("data/texture/ui/HP/HitPoint.png"));
	}

	m_pUI_ButtonHint = UI_Manager::Find<UI_ButtonHint>("ButtonHint_" + sIndex);
	m_pUI_Fade = UI_Manager::Find<UI_FadeBlack>("Fade_" + sIndex);

	m_pCharaSpawnPointManager = FindGameObject<CharaSpawnPointManager>();

	std::vector<MODEL_FRAME_TRAIL_RENDERER_DESC> descs;
	std::vector<std::pair<std::string, std::string>>* frameAndTrailNames = new std::vector<std::pair<std::string, std::string>>
	{
		{ "mixamorig:Hips", "HipsTrail" },
		{ "mixamorig:Spine2", "Spine1Trail" },
		{ "mixamorig:LeftShoulder", "LeftShoulderTrail" },
		{ "mixamorig:RightShoulder", "RightShoulderTrail" },
		{ "mixamorig:LeftLeg", "LeftLegTrail" },
		{ "mixamorig:LeftUpLeg", "LeftUpLegTrail" },
		{ "mixamorig:RightLeg", "RightLegTrail" },
		{ "mixamorig:RightUpLeg", "RightUpLegTrail"}
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
			descBold.subdivisions = 4; // 補間分割数（大きいほど滑らか）
			descBold.thick = 50.0f; // トレイルの太さ
			descBold.lifeTime = lifeTime; // トレイルの寿命
			descBold.appearRate = 0.5f; // トレイルが出現する確率（0.0f～1.0f）
			descBold.posRandomRange = Vector3(3.0f, 3.0f, 3.0f);

			MODEL_FRAME_TRAIL_RENDERER_DESC descSmall{};
			descSmall.interval = 1; // フレーム間隔（何フレームごとに描画するか）
			descSmall.subdivisions = 4; // 補間分割数（大きいほど滑らか）
			descSmall.thick = 25.0f; // トレイルの太さ
			descSmall.lifeTime = lifeTime; // トレイルの寿命
			descSmall.appearRate = 0.5f; // トレイルが出現する確率（0.0f～1.0f）
			descSmall.posRandomRange = Vector3(3.0f, 3.0f, 3.0f);

			return std::pair<MODEL_FRAME_TRAIL_RENDERER_DESC, MODEL_FRAME_TRAIL_RENDERER_DESC>(descBold, descSmall);
		};
	descs.reserve(frameAndTrailNames->size());
	for (const auto& pair : *frameAndTrailNames)
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
	delete frameAndTrailNames;

	std::string fsmname = std::string(m_User.IsHost ? "[Host] CharaFSM : " : "[Client] CharaFSM : ");
	std::string index = std::to_string(m_Index);
	m_FSM->SetName(fsmname + index);
	fsmname = std::string(m_User.IsHost ? "[Host] CharaSubFSM : " : "[Client] CharaSubFSM : ");
	m_SubFSM->SetName(fsmname + index);

	m_EffectTransform = new Transform();
	m_EffectTransform->SetParent(transform);
	m_EffectTransform->position.y = 160.0f;
	m_EffectTransform->position.z = -100.0f;
	m_EffectTransform->rotation.y = MathUtil::ToRadians(180.0f);

	m_Animator = AddComponent<Animator>();
	m_Animator->Init("mixamorig:Hips", 30.0f, 0.15f);
	m_Animator->SetOffsetMatrix(MGetRotY(DX_PI_F));

	m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");

	m_Timeline = new Timeline<Chara>(this, m_Animator);
	m_Timeline->SetFunction("SetAnimationSpeed", &Chara::setAnimationSpeed);
	m_Timeline->SetFunction("MoveToPosition", &Chara::moveToPosition);
	m_Timeline->SetFunction("MoveToWallPosition", &Chara::moveToWallPosition);
	m_Timeline->SetFunction("ChangeToRoll", &Chara::changeToRoll);
	m_Timeline->SetFunction("EndRoll", &Chara::endRoll);
	m_Timeline->SetFunction("SetCanMove", &Chara::setCanMove);
	m_Timeline->SetFunction("SetCanRot", &Chara::setCanRot);
	m_Timeline->SetFunction("SetVelocity", &Chara::setVelocity);
	m_Timeline->SetFunction("ThrowBall", &Chara::throwBall);
	m_Timeline->SetFunction("PlayFootSound", &Chara::playFootStepSound);
	m_Timeline->SetFunction("PlayTinyFootSound", &Chara::playTinyFootStepSound);
	m_Timeline->SetFunction("Invincible", &Chara::invincible);
	m_Timeline->LoadJsons("data/Json/Chara/State");

	m_BallTarget = std::make_shared<BallTarget_WithParent>(BallTarget_WithParent(Vector3::SetY(150), transform));

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

	if (net.IsNetworkEnable)
	{
		if (not m_pNetManager)
			m_pNetManager = SceneManager::CommonScene()->FindGameObject<NetworkManager>();
	}
}

void Chara::Update() {

	// デバッグ機能
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_Timeline->LoadJsons("data/Json/Chara/State");
		m_Animator->DeleteAnimInfos();
		m_Animator->LoadAnimsFromJson("data/Json/Chara/CharaAnim.json");
	}
	if (CheckHitKey(KEY_INPUT_Y))
	{
		StartRespawn();
	}

	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (m_User.UUID == m_pNetManager->g_MyUUID)
		{
			m_pNetManager->SendCharaTransform(transform->Global(), m_pNetManager->g_MyUUID);
			m_pNetManager->SendCharaAllFlag(this, m_User.UUID);
		}
	}

	// 時間が止まってたらアップデートしない
	if (GTime.DeltaTime() <= 0.0f) return;

	m_FSM->Update();
	m_SubFSM->Update();
	m_RespawnFSM->Update();
	m_Timeline->Update();

#ifdef _DEBUG
#ifdef IMGUI
	m_FSM->ImGuiDebugRender();
	m_SubFSM->ImGuiDebugRender();
#endif
#endif

	// ボールの更新
	if (m_pBall)
	{
		MATRIX m = MV1GetFrameLocalWorldMatrix(Model(), MV1SearchFrame(Model(), "mixamorig:RightHand"));
		Vector3 dir = Vector3(0, 0, 1) * MGetRotElem(m);

		//m_pBall->transform->position = Vector3(0.0f, BALL_RADIUS, -BALL_RADIUS);
		m_pBall->transform->position = VTransform(Vector3::UnitZ, m);
		m_pBall->transform->rotation = Vector3Util::DirToEuler(dir);
	}

	static const float MOVE_ACCEL = 1.8f;

	// 移動速度倍率の更新
	if (m_IsMove)
	{
		m_SpeedScale += MOVE_ACCEL * GTime.DeltaTime();
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

	HitGroundProcess();

	//=== 座標更新終了後の処理 ===

	m_BallTarget->UpdatePosition();
	m_lastUpdatePosition = transform->position;

	// NaN/Infのチェック
	if (MathUtil::IsNaNOrInf(static_cast<double>(transform->position.x)) ||
		MathUtil::IsNaNOrInf(static_cast<double>(transform->position.y)) ||
		MathUtil::IsNaNOrInf(static_cast<double>(transform->position.z)))
	{
		std::string output = "chara nan or inf : " + std::to_string(m_Index) + "\n";
		OutputDebugString(output.c_str());
		transform->position = Vector3::Zero;
		m_pPhysics->velocity = Vector3::Zero;
		m_pPhysics->resistance = Vector3::Zero;
		m_pPhysics->angularVelocity = Vector3::Zero;
	}

	//============================

	invincibleUpdate();
	buttonHintUpdate();
}

void Chara::Draw()
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

	if (m_IsWall)
	{
		DrawSphere3D(m_WallPosition, 20.0f, 4, 0xFF00FF, 0xFF00FF, FALSE);
	}
	else
	{
		DrawSphere3D(m_WallPosition, 20.0f, 4, 0x100010, 0x100010, FALSE);
	}

	DrawSphere3D(m_ActionWallPosition, 20.0f, 4, 0x00FFFF, 0x00FFFF, FALSE);
}

void Chara::CollisionEvent(const CollisionData& colData) {

	// 当たった相手がキャラクターの場合
	if (colData.Other()->Tag() == ColDefine::Tag::tChara) {

		// 相手の情報
		Chara* chara = colData.Other()->Parent<Chara>();

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
	else if (colData.Other()->Tag() == ColDefine::Tag::tBall)
	{
		// 相手の情報
		Ball* ball = colData.Other()->Parent<Ball>();

		// ボールが転がり中なら無視する
		if (ball->GetState() == Ball::S_LANDED)
		{
			// ボールを所持していない場合、自動で取得する
			if (m_pBall == nullptr)
				m_Catcher->CatchSuccese(ball);
			return;
		}

		if (ball->GetCharaTag() != m_CharaTag)
		{
			// キャッチ可能なら当たらない
			if (m_IsCatching == true && m_Catcher->CanCatch(ball))
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

			if (ball->GetLastOwner())
			{
				ball->GetLastOwner()->GetStatusTracker()->AddHitCount(1);
			}

			if (m_pHP->IsDead())
			{
				m_pStatusTracker->AddDeathCount(1);
				if (ball->GetLastOwner())
				{
					ball->GetLastOwner()->GetStatusTracker()->AddKillCount(1);
				}
				StartRespawn();
			}
		}
	}
}

void Chara::HitGroundProcess() {

	// 物理挙動
	Physics* physics = m_pPhysics;
	if (physics == nullptr)
		return;

	ColliderCapsule* capsuleCol = GetComponent<ColliderCapsule>();
	if (capsuleCol == nullptr)
		return;

	static const float DOWN_OFFSET = 0.0f;

	const float radius = capsuleCol->Radius();
	Vector3 hitPos;
	Vector3 normal;

	//=== すり抜け判定 ===
	static const float CENTER_OFFSET = 50.0f;	// 中心のオフセット
	const Vector3 moveDir = Vector3::Normalize(transform->position - m_lastUpdatePosition);
	const Vector3 centerPos = transform->position + Vector3::SetY(CENTER_OFFSET) + moveDir * radius;
	const Vector3 lastCenterPos = m_lastUpdatePosition + Vector3::SetY(CENTER_OFFSET) + moveDir * radius;

	if (StageObjectManager::CollCheckRay(lastCenterPos, centerPos, &hitPos, &normal))
	{
		Vector3 pos = (hitPos - Vector3::SetY(CENTER_OFFSET)) - moveDir * radius;
		transform->position = pos;	// レイのヒット位置へ移動
	}

	//=== 場外判定 ===
	if (StageObjectManager::CollCheck_MovableArea(centerPos))
	{
		if (m_pCharaSpawnPointManager != nullptr) {
			CharaSpawnPoint* spawnPoint = m_pCharaSpawnPointManager->Get_Near(transform->position);
			if (spawnPoint)
			{
				transform->position = spawnPoint->transform->position;
				spawnPoint->Use();
			}
			else
				transform->position = Vector3::Zero;
		}
		else
			transform->position = Vector3::SetY(CENTER_OFFSET);
	}

	//=== 地面との判定 ===s
	Vector3 headPos = capsuleCol->OffsetWorld();
	Vector3 footPos = transform->position - Vector3::SetY(DOWN_OFFSET);

	static const float RAY_START_OFFSET = -50.0f;
	static const float RAY_END_OFFSET = 100.0f;

	const Vector3 rayStart = headPos + Vector3::SetY(RAY_START_OFFSET);
	const Vector3 rayEnd = footPos - Vector3::SetY(RAY_END_OFFSET);

	if (StageObjectManager::CollCheckRay(rayStart, rayEnd, &hitPos))
	{
		if (m_pPhysics->velocity.y <= 0.0f)
		{
			land();
			transform->position = hitPos;
		}
	}
	else
	{
		m_IsLanding = false;
	}

	//=== 壁との判定 ===
	static const float CAPSULE_OFFSET = 50.0f;	// カプセルのオフセット

	Vector3 pushVec;

	if (StageObjectManager::CollCheckCapsule(footPos + Vector3::SetY(radius + CAPSULE_OFFSET), headPos - Vector3::SetY(radius), radius, &pushVec))
	{
		// 押し出しベクトルで位置を補正
		const Vector3 pushVecNorm = pushVec.Normalize();
		const float pushVecLength = pushVec.GetLength();

		transform->position += (pushVecNorm * pushVecLength);

		// Y成分が下方向なら頭上ヒット（天井にぶつかった）
		if (pushVec.y < -0.1f)
		{
			m_pPhysics->velocity.y = min(m_pPhysics->velocity.y, 0.0f);
			m_IsJumping = false;
		}
	}

	// 壁アクション判定処理
	static const float WALL_CAPSULE_RADIUS = 150.0f;

	bool wallHit = false;

	if (StageObjectManager::CollCheckCapsule(footPos + Vector3::SetY(radius), footPos + Vector3::SetY(radius), WALL_CAPSULE_RADIUS, &pushVec))
	{
		const Vector3 wallRayStart = transform->position + Vector3::SetY(CENTER_OFFSET);
		const Vector3 wallRayDir = -pushVec.Normalize();
		const float wallRayLength = WALL_CAPSULE_RADIUS * 1.5f;	// 90度の角でも当てるため、1.414倍より大きくする
		if (StageObjectManager::CollCheckRay(wallRayStart, wallRayStart + wallRayDir * wallRayLength, &hitPos, &normal))
		{
			m_WallPosition = hitPos;
			m_WallNormal = normal;
			wallHit = true;
		}
	}
	m_IsWall = wallHit;

	// 衝突していなければ、通常の空中挙動へ
	if (not m_IsLanding && not m_IsClimb)
	{
		m_pPhysics->SetGravity(CHARA_GRAVITY);
		m_pPhysics->SetFriction(Vector3::Zero);
	}
}

void Chara::climb(Vector3& normal)
{
	if (not m_CanClimb) return;
	if (m_IsLanding) return;
	if (normal.GetLengthSquared() <= 0.0f) return;

	Vector3 normalEuler = Vector3Util::DirToEuler(-normal);

	static const float ANGLE_LIMIT = MathUtil::ToRadians(30.0f);

	if (fabsf(normalEuler.x) > ANGLE_LIMIT)
	{
		return;
	}

	Vector3 vel = m_pPhysics->FlatVelocity();
	if (not vel)
	{
		vel = transform->Forward();
	}

	Vector3 velEuler = Vector3Util::DirToEuler(vel);

	float angle = normalEuler.y - velEuler.y;

	Vector3 jumpDir = Vector3(0, 0, 1) * MGetRotX(MathUtil::ToRadians(-90.0f)) * MGetRotZ(angle) * MGetRotY(normalEuler.y);

	m_pPhysics->velocity.y = 0.0f;
	m_pPhysics->velocity = jumpDir * CHARADEFINE_REF.ClimbPower;	// Magic:(

	transform->rotation.y = normalEuler.y;

	if (angle < -DX_PI_F / 4)
	{
		m_pPhysics->SetGravity(CHARA_GRAVITY / 4.0f);
		m_FSM->ChangeState(&Chara::StateWallStepRight);
	}
	else if (angle > DX_PI_F / 4)
	{
		m_pPhysics->SetGravity(CHARA_GRAVITY / 4.0f);
		m_FSM->ChangeState(&Chara::StateWallStepLeft);
	}
	else
	{
		m_FSM->ChangeState(&Chara::StateClimb);
	}
	

	m_ActionPosition = transform->position;
	m_ActionWallPosition = m_WallPosition;
	m_ActionWallNormal = m_WallNormal;
	m_CanClimb = false;
}

void Chara::Move(const Vector3& dir)
{
	auto& net = NetworkRef::Inst();
	if (not net.IsNetworkEnable)
		m_IsMove = dir.GetLengthSquared() > 0;
	else
	{
		if (net.UUID == m_User.UUID)
		{
			m_IsMove = dir.GetLengthSquared() > 0;
		}
	}

	if (m_CanRot)
	{
		float currentRot = transform->rotation.y;
		float terminusRot = atan2f(dir.x, dir.z);
		transform->rotation.y = MathUtil::RotAngle(currentRot, terminusRot, m_RotSpeed * GTime.DeltaTime());
	}

	if (!m_CanMove)
		return;

	const Vector3 normDir = Vector3::Normalize(dir);

	if (m_IsJumping && not m_IsInhibitionSpeed)
	{
		Vector3 currentVelocity = m_pPhysics->FlatVelocity();
		float speed = currentVelocity.GetLength();

		Vector3 newVelocity = normDir * speed;

		// 速度のY成分はそのまま
		m_pPhysics->velocity.x = newVelocity.x;
		m_pPhysics->velocity.z = newVelocity.z;
	}
	else
	{
		Vector3 velocity = normDir * m_MoveSpeed * m_SpeedScale * Vector3::Horizontal;

		m_pPhysics->velocity.x = velocity.x;
		m_pPhysics->velocity.z = velocity.z;
	}
}

void Chara::Jump()
{
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	m_IsJumping = true;
	m_IsInhibitionSpeed = !m_IsSliding;

	if (m_IsClimb)
	{
		m_FSM->ChangeState(&Chara::StateRunToJump);
		m_pPhysics->velocity += m_ActionWallNormal * 10.0f;	// Magic:(
		lookVelocity();

		m_CanClimb = false;
	}
}

void Chara::Slide()
{
	if (m_FSM->GetCurrentState() == &Chara::StateSlideToRun)
	{
		return;
	}

	m_SlideTimer = SLIDE_TIME;
}

void Chara::WallAction()
{
	if (not m_IsWall) return;
	if (m_IsClimb) return;

	climb(m_WallNormal);
}

void Chara::GenerateBall()
{
	if (not m_CanCatch) return;

	if (m_pBall != nullptr)
		return;

	if (m_pBallManager == nullptr)
		m_pBallManager = FindGameObject<BallManager>();

	SetBall(m_pBallManager->CreateBall(transform->Global().position));
}

void Chara::SetBall(Ball* ball)
{
	m_pBall = ball;

	if (m_pBall == nullptr)
		return;

	playPickupBallSound();

	m_pBall->transform->position = transform->Global().position;
	m_pBall->transform->rotation = transform->Global().rotation;
	m_pBall->Reset(m_CharaTag);

	m_IsCharging = false;
}

void Chara::StartBallCharge()
{
	m_IsCharging = true;
	sub_changeStateNetwork(&Chara::SubStateHoldToAim); // ステートを変更
}

void Chara::StartThrow()
{
	if (m_pBall == nullptr)
		return;
	if (not m_CanThrow)
		return;

	main_changeStateNetwork(&Chara::StateAimToThrow); // ステートを変更
}

void Chara::Feint()
{
	if (not m_CanThrow)
		return;
	main_changeStateNetwork(&Chara::StateFeint); // ステートを変更
}

void Chara::TeleportToLastBall()
{
	if (m_pLastBall == nullptr) return;
	transform->position = m_pLastBall->transform->position;

	// ToDo:消える演出
	m_pLastBall->SetIsActive(false);

	GenerateBall();

	m_pLastBall = nullptr;
	m_pPhysics->velocity.y = CHARADEFINE_REF.JumpPower;
	main_changeStateNetwork(&Chara::StateAirSpin);
}

void Chara::DropBall(const Vector3& other, float force_vertical, float force_horizontal)
{
	if (not m_pBall)
		return;

	m_pBall->ChangeState(Ball::State::S_LANDED);
	m_pBall->GetComponent<Physics>()->SetGravity(CHARA_GRAVITY);
	m_pBall->Knockback(other, force_vertical, force_horizontal);
	m_pBall->SetOwner(nullptr);
	m_pBall = nullptr;
}

void Chara::Catch()
{
	if (not m_CanCatch) return;

	if (m_pStamina->GetCurrent() > CATCH_STAMINA_MIN)
	{
		m_IsCatching = true;
	}
}

void Chara::respawn(const Vector3& pos, const Vector3& rot)
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
	m_lastUpdatePosition = transform->position;
	transform->rotation = rot;
	// キャッチャーの位置を更新
	if (m_Catcher)
	{
		m_Catcher->transform->position = Vector3(0.0f, CHARADEFINE_REF.CatchRadius, CHARADEFINE_REF.CatchRadius);
	}
}

void Chara::SetTrailImage(int hImage)
{
	m_hTrailImage = hImage;
}

Vector2 Chara::Target(const Ball* ball)
{
	return Vector2();
}

void Chara::CatchSuccess(const Vector3& velocity)
{
	m_CanCatch = false;
	m_CanMove = false;
	m_CanRot = false;
	m_Catcher->SetColliderActive(false);

	playCatchBallSound();

	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(Vector3::Zero);

	m_pPhysics->velocity.y = 10.0f;
	m_pPhysics->velocity += Vector3::Normalize(velocity) * Vector3(1, 0, 1) * 10.0f;

	sub_changeStateNetwork(&Chara::SubStateGetBall); // ステートを変更
}

void Chara::Damage(int sub) {

	m_pHP->Damage(sub);

	main_changeStateNetwork(&Chara::StateDamageToDown);

	playGetHitSound();
}

void Chara::Tackle()
{
	if (not m_CanTackle)
		return;

	m_IsTackling = true;
	const Vector3 dir = Vector3::Normalize(m_pPhysics->FlatVelocity());
	float terminusRot = atan2f(dir.x, dir.z);		// 終点の向き
	transform->rotation.y = terminusRot;
}

void Chara::GetTackle(const Vector3& other, float force_horizontal, float force_vertical, bool isForceKnockback)
{
	if (m_IsInvincible && not isForceKnockback)
	{
		return;
	}

	Knockback(other, force_vertical, force_horizontal);

	SetInvincible(CHARADEFINE_REF.TackleInvincibleDurationSec, true);

	DropBall(transform->position, BALL_REF.DropForce_Vertical, BALL_REF.DropForce_Horizontal);

	SoundManager::Play("SE_tackle_hit.mp3", "SE_tackle_hit.mp3");

	main_changeStateNetwork(&Chara::StateDamageToDown);
}

void Chara::SetInvincible(float duration_sec, bool isOverride)
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

void Chara::invincibleUpdate()
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

void Chara::StartRespawn()
{
	respawn_changeStateNetwork(&Chara::RespawnStateFadeOut); // ステートを変更
}

void Chara::Knockback(const Vector3& other, float force_vertical, float force_horizontal)
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

bool Chara::IsFinishTackleIntervalAlarm()
{
	return true;
}

void Chara::StateActionIdle(FSMSignal sig)
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
				main_changeStateNetwork(&Chara::StateActionIdleToStandingIdle); // ステートを変更
			}
			else
			{
				main_changeStateNetwork(&Chara::StateActionIdleEmote); // ステートを変更
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

void Chara::StateActionIdleEmote(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateActionIdleToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("ActionIdleToJump");
		playJumpNormalSound();
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

void Chara::StateActionIdleToRun(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("ActionIdleToRun");
		m_Timeline->Play("ActionIdleToRun");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		runUpdate();
		if (m_Animator->IsFinished())
		{
			main_changeStateNetwork(&Chara::StateRun); // ステートを変更
			//m_Animator->Play("Run");
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

void Chara::StateActionIdleToStandingIdle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateStandingIdleEmote); // ステートを変更
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

void Chara::StateActionIdleToTackle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateStandingIdleEmote); // ステートを変更
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

void Chara::StateAimToThrow(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		sub_changeStateNetwork(&Chara::SubStateNone); // ステートを変更
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateAirSpin(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateFall); // ステートを変更
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

void Chara::StateClimb(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("Climb");

		m_IsClimb = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&Chara::StateClimbToFall); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_IsClimb = false;
		m_CanMove = true;
		m_CanRot = true;
		m_Timeline->Stop();
	}
	break;
	}
}

void Chara::StateClimbToFall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("ClimbToFall");
		m_pPhysics->SetGravity(Vector3::Zero);
		m_pPhysics->velocity = m_pPhysics->FlatVelocity();
		m_IsClimb = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&Chara::StateActionIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_pPhysics->SetGravity(CHARA_GRAVITY);
		m_IsClimb = false;
		m_CanMove = true;
		m_CanRot = true;
		m_Timeline->Stop();
	}
	break;
	}
}

void Chara::StateCrouchToActionIdle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateCrouchToRun(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateRun); // ステートを変更
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

void Chara::StateDamageToDown(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateCrouchToActionIdle); // ステートを変更
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

void Chara::StateFall(FSMSignal sig)
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
				main_changeStateNetwork(&Chara::StateFallToRoll); // ステートを変更
			}
			else
			{
				main_changeStateNetwork(&Chara::StateFallToCrouch); // ステートを変更
			}
		}
		if (m_IsTackling)
		{
			main_changeStateNetwork(&Chara::StateTackle);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_SlideTimer = 0.0f;
		m_Timeline->Stop();
		m_CanTackle = false;
	}
	break;
	}
}

void Chara::StateFallToCrouch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("FallToCrouch");
		m_CanTackle = true;
		playLandingSound();
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsTackling)
		{
			main_changeStateNetwork(&Chara::StateTackle);
		}

		if (m_pPhysics->FlatVelocity().GetLengthSquared() > 0)
		{
			main_changeStateNetwork(&Chara::StateCrouchToRun); // ステートを変更
		}
		else
		{
			main_changeStateNetwork(&Chara::StateCrouchToActionIdle); // ステートを変更
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

void Chara::StateFallToRoll(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("FallToRoll");
		m_CanTackle = true;
		playLandingSound();
		playLandingRollSound();
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_IsTackling)
		{
			main_changeStateNetwork(&Chara::StateTackle);
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

void Chara::StateFeint(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		sub_changeStateNetwork(&Chara::SubStateNone); // ステートを変更
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		sub_changeStateNetwork(&Chara::SubStateHoldToAim); // ステートを変更

		m_CanCatch = true;
		m_CanHold = true;
		m_CanThrow = true;
		m_CanTackle = true;

		m_Timeline->Stop();
	}
	break;
	}
}

void Chara::StateRoll(FSMSignal sig)
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

void Chara::StateRollToActionIdle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateRollToRun(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateRun); // ステートを変更

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

void Chara::StateRun(FSMSignal sig)
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

void Chara::StateRunToActionIdle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateRunToJump(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("RunToJump");
		playJumpDashSound();
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

void Chara::StateRunToSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("RunToSlide");
		m_pPhysics->velocity = m_pPhysics->UpVelocity() + m_pPhysics->FlatVelocity() * 2.0f;

		if (not m_FSM->HasTransitionWithInThePast(&Chara::StateSlide, 1))
			m_pPhysics->velocity = m_pPhysics->velocity * 1.5f;

		m_IsSliding = true;
		m_CanMove = false;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		slideUpdate();
		if (m_Animator->IsFinished())
		{
			main_changeStateNetwork(&Chara::StateSlide); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_IsSliding = false;
		m_CanMove = true;
	}
	break;
	}
}

void Chara::StateRunToTackle(FSMSignal sig)
{
}

void Chara::StateSlide(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->Play("Slide");
		m_CanMove = false;
		m_IsSliding = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		slideUpdate();
		if (m_SlideTimer <= 0.0f)
		{
			main_changeStateNetwork(&Chara::StateSlideToRun); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate: // 更新後の更新
	{
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_SlideTimer = 0.0f;
		m_CanMove = true;
		m_IsSliding = false;
	}
	break;
	}
}

void Chara::StateSlideToRun(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateRun); // ステートを変更
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

void Chara::StateStandingIdle(FSMSignal sig)
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

void Chara::StateStandingIdleEmote(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateStandingIdleToActionIdle); // ステートを変更
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

void Chara::StateStandingIdleToActionIdle(FSMSignal sig)
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
			main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateTackle(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("Tackle");

		m_CanMove = false;
		m_CanRot = false;

		m_Tackler->SetColliderActive(true);

		playTackleSound();

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
				main_changeStateNetwork(&Chara::StateActionIdle); // ステートを変更
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

void Chara::StateWallStepLeft(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("WallStepLeft");

		m_IsClimb = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&Chara::StateFall); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_IsClimb = false;
		m_CanMove = true;
		m_CanRot = true;
		m_pPhysics->SetGravity(CHARA_GRAVITY);
		m_Timeline->Stop();
	}
	break;
	}
}

void Chara::StateWallStepRight(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Timeline->Play("WallStepRight");

		m_IsClimb = true;
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinished())
		{
			m_FSM->ChangeState(&Chara::StateFall); // ステートを変更
		}
	}
	break;
	case FSMSignal::SIG_Exit: // 終了
	{
		m_IsClimb = false;
		m_CanMove = true;
		m_CanRot = true;
		m_pPhysics->SetGravity(CHARA_GRAVITY);
		m_Timeline->Stop();
	}
	break;
	}
}

//========================================================================
// サブステート

void Chara::SubStateNone(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->StopSub("mixamorig:Spine");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_CanCatch && m_IsCatching)
		{
			sub_changeStateNetwork(&Chara::SubStateCatch); // ステートを変更
		}
		else if (m_CanHold && m_pBall != nullptr)
		{
			sub_changeStateNetwork(&Chara::SubStateHold); // ステートを変更
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

void Chara::SubStateGetBall(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:Spine", "GetBall");
		m_CanMove = false;
		m_CanRot = false;
		m_pPhysics->SetGravity(Vector3::Zero);
		m_pPhysics->SetFriction(Vector3::Zero);
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_Animator->IsFinishedSub("mixamorig:Spine"))
		{
			sub_changeStateNetwork(&Chara::SubStateHold); // ステートを変更
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
		m_pPhysics->SetGravity(CHARA_GRAVITY);
		m_pPhysics->SetFriction(FRICTION);
	}
	break;
	}
}

void Chara::SubStateHold(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:Spine", "Hold");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (m_pBall == nullptr)
		{
			sub_changeStateNetwork(&Chara::SubStateNone); // ステートを変更
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

void Chara::SubStateHoldToAim(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:Spine", "HoldToAim");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		m_BallChargeRate += GTime.deltaTime / CHARGE_TIME;
		if (m_BallChargeRate > 1.0f)
		{
			m_BallChargeRate = 1.0f;
		}

		// カメラの向きに合わせる
		m_CanRot = false;

		Camera* camera = CameraManager::GetCamera(m_Index);
		if (camera != nullptr) {
			float currentY = transform->rotation.y;
			float terminusY = camera->transform->rotation.y;
			transform->rotation.y = MathUtil::LerpAngle(currentY, terminusY, 0.5f);
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

void Chara::SubStateCatch(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter: // 開始
	{
		m_Animator->PlaySub("mixamorig:Spine", "Catch");
	}
	break;
	case FSMSignal::SIG_Update: // 更新
	{
		if (not m_IsCatching)
		{
			sub_changeStateNetwork(&Chara::SubStateNone); // ステートを変更
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

void Chara::land()
{
	m_IsLanding = true;
	m_IsJumping = false;
	m_CanClimb = true;
	m_pPhysics->velocity.y = 0.0f;
	m_pPhysics->resistance.y = 0.0f;
	m_pPhysics->SetGravity(Vector3::Zero);
	m_pPhysics->SetFriction(FRICTION);
}

void Chara::idleUpdate()
{
	if (m_IsJumping)
	{
		main_changeStateNetwork(&Chara::StateActionIdleToJump); // ステートを変更
	}
	else if (not m_IsLanding)
	{
		main_changeStateNetwork(&Chara::StateFall); // ステートを変更
	}
	else if (m_IsMove)
	{
		main_changeStateNetwork(&Chara::StateActionIdleToRun); // ステートを変更
	}
	else if (m_IsTackling)
	{
		main_changeStateNetwork(&Chara::StateTackle);
	}
}

void Chara::runUpdate()
{
	if (m_IsJumping)
	{
		main_changeStateNetwork(&Chara::StateRunToJump); // ステートを変更
	}
	else if (not m_IsLanding)
	{
		main_changeStateNetwork(&Chara::StateFall); // ステートを変更
	}
	else if (not m_IsMove)
	{
		main_changeStateNetwork(&Chara::StateRunToActionIdle); // ステートを変更
	}
	else if (m_SlideTimer > 0.0f)
	{
		main_changeStateNetwork(&Chara::StateRunToSlide); // ステートを変更
	}
	else if (m_IsTackling)
	{
		main_changeStateNetwork(&Chara::StateTackle);
	}
}

void Chara::slideUpdate()
{
	if (m_pPhysics->velocity.y > 0.0f)
	{
		main_changeStateNetwork(&Chara::StateRunToJump); // ステートを変更
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
		main_changeStateNetwork(&Chara::StateTackle);
	}
}

void Chara::catchUpdate()
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

void Chara::jumpUpdate()
{
	if (m_Animator->IsFinished())
	{
		m_IsJumping = false;
	}

	if (not m_IsJumping)
	{
		main_changeStateNetwork(&Chara::StateFall); // ステートを変更
	}

	if (m_IsTackling)
	{
		main_changeStateNetwork(&Chara::StateTackle);
	}
}

void Chara::tackleUpdate()
{
	if (m_Animator->IsFinished())
	{
		// あと隙
		m_Alarm->Set(0.1f); // magic:>
	}
}

void Chara::getHit(Ball* hit)
{
	main_changeStateNetwork(&Chara::StateDamageToDown);

	Vector3 dif = hit->GetComponent<Physics>()->velocity * -1.0f;

	m_pHitBall = hit;

	float forwardRad = atan2f(dif.x, dif.z);
	transform->rotation.y = forwardRad;
	m_pPhysics->velocity += transform->Forward() * -50.0f;	// ToDo:外部化

	playGetHitSound();
}

void Chara::throwBallForward()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 dir = Vector3::Normalize(forward + Vector3::SetY(0.3f));	// Magic:)

	m_pBall->ThrowDirection(dir, this, m_BallChargeRate);

	releaseBall();
}

void Chara::throwBallHoming()
{
	if (m_pBall == nullptr)
		return;

	Vector3 forward = transform->Forward();
	Vector3 dir = Vector3::Normalize(forward + Vector3::SetY(0.3f));	// Magic:)

	const Chara* targetChara = nullptr;
	Camera* camera = CameraManager::GetCamera(m_Index);

	if (camera != nullptr)
	{
		targetChara = camera->TargetChara();	// カメラのターゲットキャラを取得
	}

	if (targetChara != nullptr)
	{
		std::shared_ptr<BallTarget> target = targetChara->GetBallTarget();

		if (m_IsLanding == true)
		{
			m_pBall->ThrowHoming(std::move(target), this, m_BallChargeRate, 0.0f, 0.0f);	// Magic:)
		}
		else
		{
			// 自分の向きとターゲットの向きを比較して、投げる角度を調整
			Vector3 targetDir = Vector3::Normalize(target->Position() - transform->position);
			float angle = Vector3Util::Vec2ToRad(targetDir.z, targetDir.x) - Vector3Util::Vec2ToRad(dir.z, dir.x);

			// 角度を90度単位で丸める
			float angleRound = roundf(angle / (DX_PI_F * 0.5f));
			angle = angleRound * (DX_PI_F * 0.5f);

			m_pBall->ThrowHoming(std::move(target), this, m_BallChargeRate, angle, 0.5f);	// Magic:)
		}
	}
	else
	{
		m_pBall->Throw(this, m_BallChargeRate);	// Magic:)

		Physics* pBallPhysics = m_pBall->GetComponent<Physics>();
		if (pBallPhysics != nullptr) {
			int chargeLevel = 0;

			if (m_BallChargeRate < 0.5f)
					 chargeLevel = 0;
			else if (m_BallChargeRate < 1.0f)
				chargeLevel = 1;
			else
				chargeLevel = 2;
			
			pBallPhysics->velocity += forward.Normalize() * BALL_REF.ChargeLevels[chargeLevel].Speed * GTime.DeltaTime();
		}
	}

	releaseBall();
}

void Chara::releaseBall()
{
	m_pStatusTracker->AddThrowCount(1);

	playThrowSound();

	m_pLastBall = m_pBall;
	m_pBall = nullptr;
	m_IsCharging = false;
	m_BallChargeRate = 0.0f;
}

void Chara::respawnByPoint()
{
	CharaSpawnPoint* csp = m_SpawnPointManager->Get_LowUsageRate();
	Vector3 position = csp->transform->position;
	respawn(position, Vector3::Zero);
	csp->Use();
}

void Chara::lookVelocity()
{
	transform->rotation.y = Vector3Util::DirToEuler(m_pPhysics->velocity);
}

void Chara::playThrowSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_throw_";
	std::string power = "";
	std::string num = "00";
	std::string fileType = ".mp3";

	std::string impactSoundName = "SE_throw_impact_";
	std::string impactPower = "normal";

	if (m_BallChargeRate < 0.5f)
	{
		power += "weak_";
	}
	else if (m_BallChargeRate < 1.0f)
	{
		power += "normal_";
	}
	else
	{
		impactPower = "strong";
		power += "strong_";
	}

	if (rand < 50)
	{
		num = "01";
	}

	std::string soundName = base + power + num + fileType;
	std::string soundName2 = impactSoundName + impactPower + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
	if (not SoundManager::IsPlaying(soundName2, soundName2))
		SoundManager::Play(soundName2, soundName2);
}

void Chara::playGetHitSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_hit_";
	std::string num = "00";
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

void Chara::playCatchBallSound()
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

void Chara::playPickupBallSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_pickup_";
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

	std::string soundName = base + num + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void Chara::playVacuumSound()
{
}

void Chara::playJumpNormalSound()
{
	std::string base = "SE_jump_normal";
	std::string fileType = ".mp3";

	std::string soundName = base + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void Chara::playJumpDashSound()
{
	std::string base = "SE_jump_dash";
	std::string fileType = ".mp3";

	std::string soundName = base + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void Chara::playLandingSound()
{
	std::string base = "SE_landing";
	std::string fileType = ".mp3";

	std::string soundName = base + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void Chara::playLandingRollSound()
{
	std::string base = "SE_landing_roll";
	std::string fileType = ".mp3";

	std::string soundName = base + fileType;

	if (not SoundManager::IsPlaying(soundName, soundName))
		SoundManager::Play(soundName, soundName);
}

void Chara::playTackleSound()
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_landing_roll";
	std::string fileType = ".mp3";
	std::string num = "";

	if (rand < 50)
	{
		num = "00";
	}
	else
	{
		num = "01";
	}

	std::string soundName = base + fileType;

	SoundManager::Play(soundName, soundName);
}

void Chara::setAnimationSpeed(const nlohmann::json& argument)
{
	m_Animator->SetPlaySpeed(argument.at("Speed").get<float>());
}

void Chara::moveToPosition(const nlohmann::json& argument)
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

void Chara::moveToWallPosition(const nlohmann::json& argument)
{
	float progress = argument.at("Progress").get<float>();
	float lastProgress = argument.at("LastProgress").get<float>();
	Vector3 dest = m_ActionWallPosition - m_ActionPosition;

	Vector3 add = dest * progress;
	Vector3 sub = dest * lastProgress;

	transform->position += add;
	transform->position -= sub;
}

void Chara::changeToRoll(const nlohmann::json& argument)
{
	main_changeStateNetwork(&Chara::StateRoll); // ステートを変更
}

void Chara::endRoll(const nlohmann::json& argument)
{
	if (m_IsMove)
	{
		main_changeStateNetwork(&Chara::StateRollToRun); // ステートを変更
	}
	else
	{
		main_changeStateNetwork(&Chara::StateRollToActionIdle); // ステートを変更
	}
}

void Chara::setCanMove(const nlohmann::json& argument)
{
	m_CanMove = argument.at("CanMove").get<bool>();
	m_CanRot = m_CanMove;
}

void Chara::setCanRot(const nlohmann::json& argument)
{
	m_CanRot = argument.at("CanRot").get<bool>();
}

void Chara::setVelocity(const nlohmann::json& argument)
{
	m_pPhysics->velocity = argument.at("Velocity").get<Vector3>();
}

void Chara::throwBall(const nlohmann::json& argument)
{
	if (m_FSM->GetCurrentState() == &Chara::StateAimToThrow)
	{
		throwBallHoming();
	}
}

void Chara::invincible(const nlohmann::json& argument)
{
	std::shared_ptr<BallTarget_WithParent> copy(new BallTarget_WithParent(*m_BallTarget));
	m_BallTarget.reset();
	m_BallTarget = std::move(copy);
}

void Chara::playFootStepSound(const nlohmann::json& argument)
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_footsound_";
	std::string num = "00";
	std::string fileType = ".mp3";

	if (rand < 25)
	{
		num = "00";
	}
	else if (rand < 50)
	{
		num = "01";
	}
	else if (rand < 75)
	{
		num = "02";
	}
	else
	{
		num = "03";
	}

	std::string soundName = base + num + fileType;

	SoundManager::Play(soundName, soundName);
}

void Chara::playTinyFootStepSound(const nlohmann::json& argument)
{
	int rand = Random.GetIntRange(0, 100);

	std::string base = "SE_tiny_footsound_";
	std::string num = "00";
	std::string fileType = ".mp3";

	if (rand < 25)
	{
		num = "00";
	}
	else if (rand < 50)
	{
		num = "01";
	}
	else if (rand < 75)
	{
		num = "02";
	}
	else
	{
		num = "03";
	}

	std::string soundName = base + num + fileType;

	SoundManager::Play(soundName, soundName);
}

void Chara::main_changeStateNetwork(void(Chara::* state)(FSMSignal sig))
{
	m_FSM->ChangeState(state);
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.UUID == m_User.UUID)
			sendChangeStateToNetwork(m_FSM->GetStateNameFromMap(state));
	}
}

void Chara::sub_changeStateNetwork(void(Chara::*state)(FSMSignal sig))
{
	m_SubFSM->ChangeState(state);
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.UUID == m_User.UUID)
			sendChangeSubStateToNetwork(m_FSM->GetStateNameFromMap(state));
	}
}

void Chara::respawn_changeStateNetwork(void(Chara::*state)(FSMSignal sig))
{
	m_RespawnFSM->ChangeState(state);
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.UUID == m_User.UUID)
		{

		}
	}
}


void Chara::sendChangeStateToNetwork(const std::string& state)
{
	auto& net = NetworkRef::Inst();
	if (not net.IsNetworkEnable)
		return;
	m_pNetManager->SendCharaChangeState(state, m_User.UUID);
}

void Chara::sendChangeSubStateToNetwork(const std::string& state)
{
	auto& net = NetworkRef::Inst();
	if (not net.IsNetworkEnable)
		return;
	m_pNetManager->SendCharaChangeSubState(state, m_User.UUID);
}

void Chara::sendChangeRespawnStateToNetwork(const std::string& state)
{
	auto& net = NetworkRef::Inst();
	if (not net.IsNetworkEnable)
		return;
	m_pNetManager->SendCharaChangeSubState(state, m_User.UUID);
}

void Chara::buttonHintUpdate()
{
	if (m_pUI_ButtonHint == nullptr)
		return;

	// ボタンヒント
	{
		if (m_CanCatch)
		{
			if(not m_IsCatching)
				m_pUI_ButtonHint->Activate("LeftTrigger");
			else 
				m_pUI_ButtonHint->PushKey("LeftTrigger");
		}
		else
			m_pUI_ButtonHint->Deactivate("LeftTrigger");

		if (m_CanTackle)
			m_pUI_ButtonHint->Activate("ButtonB");
		else
			m_pUI_ButtonHint->Deactivate("ButtonB");

		if (m_CanThrow)
		{
			if (not m_IsCharging)
				m_pUI_ButtonHint->Activate("RightTrigger");
			else
				m_pUI_ButtonHint->PushKey("RightTrigger");

			m_pUI_ButtonHint->Activate("ButtonX");
		}
		else
			m_pUI_ButtonHint->Deactivate("RightTrigger");

		if (m_IsLanding)
		{
			m_pUI_ButtonHint->Activate("ButtonA");

			if (not m_IsSliding)
				m_pUI_ButtonHint->Activate("LeftShoulder");
			else
				m_pUI_ButtonHint->PushKey("LeftShoulder");
		}
		else
			m_pUI_ButtonHint->Deactivate("ButtonA");
	}
}