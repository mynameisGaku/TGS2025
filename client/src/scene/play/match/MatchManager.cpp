#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>
#include <src/util/debug/imgui/imGuiManager.h>
#include "src/util/history/History.h"

//=== キャラ ===
#include "src/util/math/MathUtil.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"
#include "src/scene/play/status_tracker/StatusTracker.h"

//=== スポナー ===
#include "src/scene/play/chara/CharaSpawnPointManager.h"

//=== チーム ===
#include "src/scene/play/team/Team.h"
#include "src/scene/play/team/TeamManager.h"

//=== ボール ===
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include <src/util/string/StringUtil.h>

//=== カメラ ===
#include "src/common/camera/CameraManager.h"

//=== UI ===
#include "src/scene/play/ui/UI_GameScore.h"
#include "src/util/screen/ScreenManager.h"

#include "src/util/color/ColorUtil.h"

#include "src/util/input/InputManager.h"
#include "src/util/fader/Fader.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>
#include <src/scene/play/ui/UI_Setter_PlayScene.h>
#include <src/common/stage/StageObjectManager.h>
#include <src/common/stage/SpawnerObjectQueue.h>
#include <src/scene/play/ball/BallSpawner.h>

//-----------------------------------------
//  CurrentGameData
//-----------------------------------------
CurrentGameData::CurrentGameData(const GAME_MODE_DESC& desc, std::vector<std::string> teamNames)
{
	m_Name = desc.GameModeName;
	m_WinPointMax = desc.WinPointMax;
	m_PlayTimeMaxSec = desc.PlayTimeMaxSec;
	m_PlayTimeCounterSec = 0.0f;

	for (auto& teamName : teamNames)
	{
		m_TeamPointMap[teamName] = 0;
	}
}

//-----------------------------------------
//  MatchManager
//-----------------------------------------
MatchManager::MatchManager()
{
	init();
}

MatchManager::~MatchManager()
{
	delete m_pFsm;

	if (m_UI_GameScore != nullptr)
	{
		m_UI_GameScore->SetMatchManager(nullptr);
		delete m_UI_GameScore;
	}

	if (m_UI_Result_Won != nullptr)
	{
		delete m_UI_Result_Won;
	}

	if (m_UI_Result_Lost != nullptr)
	{
		delete m_UI_Result_Lost;
	}
}

void MatchManager::Update()
{
	if (m_pTeamManager && m_UI_GameScore)
	{
		for (const auto& team : m_pTeamManager->GetTeams())
		{
			for (int id : team->GetCharaIDs())
			{
				const std::string teamName = team->GetTeamName();

				const Chara* chara = m_pCharaManager->GetCharaPool()->GetItem(id)->m_pObject;
			   
				if (chara == nullptr)
					continue;  // キャラが存在しない場合はスキップ
				if (chara->GetStatusTracker() == nullptr)
					continue;  // ステータストラッカーが存在しない場合はスキップ

				int killCount = chara->GetStatusTracker()->Get_KillCount();

				m_UI_GameScore->SetUserScore(teamName, id, killCount);
			}
		}

		m_UI_GameScore->SetAchievedScore(m_GameData.m_WinPointMax);
	}

	m_pFsm->Update();

#ifdef IMGUI
	ImGuiRoot* matchRoot = ImGuiManager::FindRoot("Match");
	if (!matchRoot) return;

	for (auto& team : m_pTeamManager->GetTeams())
	{
		std::string teamName = team->GetTeamName();
		ImGuiRoot* teamTree = matchRoot->SearchChildren(teamName + " Team");
		if (!teamTree) continue;

		// 合計ポイントの更新
		teamTree->Node<ImGuiNode_Text>("TotalPoint")->SetText(
			StringUtil::FormatToString("Total Point: %d", team->GetTotalPoint())
		);

		// ランキングの更新
		ImGuiRoot* rankingTree = teamTree->SearchChildren("Ranking");
		if (rankingTree)
		{
			size_t i = 0;
			for (auto& rank : team->GetTeamRanking())
			{
				rankingTree->Node<ImGuiNode_Text>("Rank" + std::to_string(i))->SetText(
					StringUtil::FormatToString("[%d] ID:%d | Point:%d", i + 1, rank.first, rank.second)
				);
				++i;
			}
		}

		// キャラクターごとのステータス更新
		ImGuiRoot* memberTree = teamTree->SearchChildren("Members");
		if (!memberTree) continue;

		for (auto& id : team->GetCharaIDs())
		{
			auto chara = m_pCharaManager->GetCharaPool()->GetItem(id)->m_pObject;
			auto stat = chara->GetStatusTracker();

			std::string label = "[" + std::to_string(chara->GetIndex()) + "] ID:" + std::to_string(chara->GetIndex());
			ImGuiRoot* charaTree = memberTree->SearchChildren(label);
			if (!charaTree) continue;

			// 数値がNaNやInfの場合は 0.0f に補正
			float kd = MathUtil::IsNaN(stat->GetKD()) || MathUtil::IsInf(stat->GetKD()) ? 0.0f : stat->GetKD();
			float acc = MathUtil::IsNaN(stat->GetAccuracy()) || MathUtil::IsInf(stat->GetAccuracy()) ? 0.0f : stat->GetAccuracy();

			charaTree->Node<ImGuiNode_Text>("KD")->SetText(StringUtil::FormatToString("KD: %.2f", kd));
			charaTree->Node<ImGuiNode_Text>("Acc")->SetText(StringUtil::FormatToString("Acc: %.2f", acc));
			charaTree->Node<ImGuiNode_Text>("kill")->SetText(StringUtil::FormatToString("Kill: %d", stat->Get_KillCount()));
			charaTree->Node<ImGuiNode_Text>("death")->SetText(StringUtil::FormatToString("Death: %d", stat->Get_DeathCount()));
			charaTree->Node<ImGuiNode_Text>("hit")->SetText(StringUtil::FormatToString("Hit: %d", stat->Get_HitCount()));
			charaTree->Node<ImGuiNode_Text>("gethit")->SetText(StringUtil::FormatToString("GetHit: %d", stat->Get_GetHitCount()));
			charaTree->Node<ImGuiNode_Text>("catch")->SetText(StringUtil::FormatToString("Catch: %d", stat->Get_CatchCount()));
			charaTree->Node<ImGuiNode_Text>("throw")->SetText(StringUtil::FormatToString("Throw: %d", stat->Get_ThrowCount()));
		}
	}
#endif
}

void MatchManager::Draw()
{
	//if (not CameraManager::IsScreenDivision())
	//    m_pFsm->ImGuiDebugRender();
}

void MatchManager::ReloadCurrentGameData()
{
	m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());
}

std::string MatchManager::GetWinnerTeamName()
{
	return m_GameData.m_WinnerTeam == "" ? "No Winner." : m_GameData.m_WinnerTeam;
}

float MatchManager::GetPlayTimeSec()
{
	return m_PlayTimeCounter;
}

float MatchManager::GetPlayTimeMaxSec()
{
	return m_GameData.m_PlayTimeMaxSec;
}

int MatchManager::GetWinPointMax()
{
	return m_GameData.m_WinPointMax;
}

float MatchManager::GetReadyTimerSec()
{
	return m_ReadyEndCounterSec;
}

std::vector<std::pair<int, int>> MatchManager::GetRanking() const {

	std::vector<std::pair<int, int>> ranking;

	for (const auto chara : m_pCharaManager->GetCharaPool()->GetAllItems())
	{
		if (chara->m_pObject == nullptr)
			continue;  // キャラが存在しない場合はスキップ

		if (chara->m_pObject->GetStatusTracker() == nullptr)
			continue;  // ステータストラッカーが存在しない場合はスキップ

		int charaPoints = chara->m_pObject->GetStatusTracker()->Get_KillCount();
		ranking.emplace_back(chara->m_Index, charaPoints);
	}

	// ポイントの降順でソート  
	std::sort(ranking.begin(), ranking.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
		return a.second > b.second;
		});

	return ranking;
}

const std::string MatchManager::GetTeamName(int charaID) const {

	std::string teamName = "";

	if (m_pTeamManager == nullptr)
		return teamName;

	teamName = m_pTeamManager->GetTeamName(charaID);

	return teamName;
}

void MatchManager::init()
{
	m_pFsm = new TinyFSM<MatchManager>(this);
	m_pFsm->ChangeState(&MatchManager::StatePhaseBegin);
#if _DEBUG
	m_pFsm->SetName("MatchManager FSM");
	m_pFsm->RegisterStateName(&MatchManager::StatePhaseBegin, "Begin");
	m_pFsm->RegisterStateName(&MatchManager::StatePhaseReady, "Ready");
	m_pFsm->RegisterStateName(&MatchManager::StatePhasePlay, "Play");
	m_pFsm->RegisterStateName(&MatchManager::StatePhaseGameOver, "GameOver");
	m_pFsm->RegisterStateName(&MatchManager::StatePhaseEnd, "End");
#endif

	m_IsFadeEnd = false;
}

void MatchManager::ImGuiInit()
{
#ifdef _DEBUG
	ImGuiRoot* matchRoot = ImGuiManager::AddRoot(new ImGuiRoot("Match"));  // マッチのトップ
	std::function<void()> func = [&] {ReloadCurrentGameData(); };
	matchRoot->Add(new ImGuiNode_Button("Reload GameData", func));

	for (auto& team : m_pTeamManager->GetTeams())
	{
		std::string teamName = team->GetTeamName();
		ImGuiRoot* teamTree = matchRoot->AddChild(new ImGuiRoot(teamName + " Team"));

		// ランキング
		ImGuiRoot* rankingTree = teamTree->AddChild(new ImGuiRoot("Ranking"));
		size_t rankIndex = 0;
		rankingTree->NodeBeginChild(250, 100);
		for (auto& rank : team->GetTeamRanking())
		{
			rankingTree->Add(new ImGuiNode_Text("Rank" + std::to_string(rankIndex++),
				StringUtil::FormatToString("[%d] ID:%d | Point:%d", rankIndex, rank.first, rank.second)));
		}
		rankingTree->NodeEndChild();

		// キャラ情報
		ImGuiRoot* memberTree = teamTree->AddChild(new ImGuiRoot("Members"));
		for (auto& id : team->GetCharaIDs())
		{
			auto chara = m_pCharaManager->GetCharaPool()->GetItem(id)->m_pObject;
			auto stat = chara->GetStatusTracker();

			std::string label = "[" + std::to_string(chara->GetIndex()) + "] ID:" + std::to_string(chara->GetIndex());

			ImGuiRoot* charaTree = memberTree->AddChild(new ImGuiRoot(label));
			charaTree->NodeBeginChild(250, 100);
			charaTree->Add(new ImGuiNode_Text("KD", StringUtil::FormatToString("KD: %.2f", stat->GetKD())));
			charaTree->Add(new ImGuiNode_Text("Acc", StringUtil::FormatToString("Acc: %.2f", stat->GetAccuracy())));
			charaTree->Add(new ImGuiNode_Text("kill", StringUtil::FormatToString("Kill: %d", stat->Get_KillCount())));
			charaTree->Add(new ImGuiNode_Text("death", StringUtil::FormatToString("Death: %d", stat->Get_DeathCount())));
			charaTree->Add(new ImGuiNode_Text("hit", StringUtil::FormatToString("Hit: %d", stat->Get_HitCount())));
			charaTree->Add(new ImGuiNode_Text("gethit", StringUtil::FormatToString("GetHit: %d", stat->Get_GetHitCount())));
			charaTree->Add(new ImGuiNode_Text("catch", StringUtil::FormatToString("Catch: %d", stat->Get_CatchCount())));
			charaTree->Add(new ImGuiNode_Text("throw", StringUtil::FormatToString("Throw: %d", stat->Get_ThrowCount())));
			charaTree->NodeEndChild();
		}

		// 合計ポイント
		teamTree->Add(new ImGuiNode_Space("Space"));
		teamTree->Add(new ImGuiNode_Text("TotalPoint", StringUtil::FormatToString("Total Point: %d", team->GetTotalPoint())));
		teamTree->Add(new ImGuiNode_Space("Space"));
	}
#endif
}

/* 以下ステート */

void MatchManager::StatePhaseBegin(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter:
	{
		/* ここで準備を行う */
		m_pCharaManager = Instantiate<CharaManager>();

		m_pTeamManager = Instantiate<TeamManager>();

		auto spawnerManager = FindGameObject<CharaSpawnPointManager>();
		auto spawner1 = spawnerManager->Get_Near(Vector3(500, 0, 500));
		auto spawner2 = spawnerManager->Get_Near(Vector3(-500, 0, -500));


		// Networkが有効なら
		auto& net = NetworkRef::Inst();
		if (net.IsNetworkEnable)
		{
			NetworkManager* netManager = SceneManager::CommonScene()->FindGameObject<NetworkManager>();

			// ホストなら
			if (net.IsHost)
			{
				// スポナー生成する
				auto queue = StageObjectManager::GetBallSpawnerQueue();
				for (auto& bsdesc : queue->Get())
				{
					netManager->SendAddBallSpawner(bsdesc.modelHandle, bsdesc.transform, bsdesc.desc, bsdesc.id);
					AddBallSpawner(bsdesc.modelHandle, bsdesc.transform, bsdesc.desc, bsdesc.id);
				}
			}

			// NetworkManagerに登録されているg_Usersを元にキャラを生成する
			{
				UINT i = 0;
				for (auto& user : netManager->g_Users)
				{
					i++;

					std::string teamColor = "Red";
					CharaSpawnPoint* useSpawner = spawner1;
					if (i % 2 == 0)
					{
						teamColor = "Blue";
						useSpawner = spawner2;
					}

					auto c = addCharacter(user, teamColor, useSpawner->transform->Global(), false);

					// このクライアントで動作しているキャラ以外のカメラは生成しない。
					if (user.UUID == net.UUID)
						CameraManager::CreateCamera(c->GetIndex(), user);
				}
			}
		}
		else
		{
			addCharacter("Red", Transform(spawner1->transform->position + Vector3::SetY(100.0f), Vector3::Zero, Vector3::Ones), false);
			addCharacter("Blue", Transform(spawner2->transform->position + Vector3::SetY(100.0f), Vector3::Zero, Vector3::Ones), false);
		}

		// 追加できるの確認したよ
		//addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 150.0f), Vector3::Zero, Vector3::Ones), false);
		//addCharacter("Blue", Transform(Vector3(250.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);
		//addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 250.0f), Vector3::Zero, Vector3::Ones), false);
		//addCharacter("Blue", Transform(Vector3(350.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);

		m_pBallManager = Instantiate<BallManager>();

		// 現在のゲームモードとそのデータを取得し、取っておく
		m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());

		ImGuiInit();

		m_UI_GameScore = new UI_GameScore(RectTransform(Anchor::Preset::MiddleUp));
		m_UI_GameScore->SetMatchManager(this);
		m_UI_GameScore->SetAchievedScore(m_GameData.m_WinPointMax);

		for (const auto& team : m_pTeamManager->GetTeams())
		{
			for (int id : team->GetCharaIDs())
			{
				const std::string teamName = team->GetTeamName();

				const Chara* chara = m_pCharaManager->GetCharaPool()->GetItem(id)->m_pObject;
				if (chara == nullptr)
					continue;  // キャラが存在しない場合はスキップ
				if (chara->GetStatusTracker() == nullptr)
					continue;  // ステータストラッカーが存在しない場合はスキップ

				int killCount = chara->GetStatusTracker()->Get_KillCount();

				m_UI_GameScore->AddUserScore(teamName, id, killCount, ColorUtil::ColorFromString(teamName));
			}
		}
	}
	break;
	case FSMSignal::SIG_Update:
	{
		m_pFsm->ChangeState(&MatchManager::StatePhaseReady);
	}
	break;
	}
}

void MatchManager::StatePhaseReady(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter:
	{
		m_ReadyEndCounterSec = GAME_REF.GameStartCountMaxSec;
	}
	break;
	case FSMSignal::SIG_Update:
	{
		if (m_ReadyEndCounterSec <= 0.0f)
		{
			m_pFsm->ChangeState(&MatchManager::StatePhasePlay);
		}
		else
			m_ReadyEndCounterSec -= GTime.deltaTime;
	}
	break;
	}
}

void MatchManager::StatePhasePlay(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter:
	{

	}
	break;
	case FSMSignal::SIG_Update:
	{
		m_PlayTimeCounter += GTime.deltaTime;

		if (m_PlayTimeCounter >= m_GameData.m_PlayTimeMaxSec)
		{
			m_pFsm->ChangeState(&MatchManager::StatePhaseGameOver);
		}

		for (auto& teamName : GAME_REF.TeamNames)
		{
			if (m_pTeamManager->GetTeam(teamName)->GetTotalPoint() < m_GameData.m_WinPointMax)
				continue;

			m_pFsm->ChangeState(&MatchManager::StatePhaseGameOver);
		}
	}
	break;
	case FSMSignal::SIG_AfterUpdate:
	{
	}
	break;
	case FSMSignal::SIG_Exit:
	{
		// ゲーム終了時に、勝利チームの情報をGameManagerに渡す
		GameManager::ResultData resultData;

		for (auto& teamName : GAME_REF.TeamNames)
		{
			Team* team = m_pTeamManager->GetTeam(teamName);
			if (team->GetTotalPoint() >= m_GameData.m_WinPointMax)
			{
				resultData.WinnerTeamName.push_back(teamName);
				resultData.WinnerCharaIDs = team->GetCharaIDs();
			}
			resultData.TeamName.push_back(teamName);
			resultData.TeamTotalPoint[teamName] = team->GetTotalPoint();
			resultData.TeamColor[teamName]      = (ColorUtil::ColorFromString(teamName));

			for (const auto& charaID : team->GetCharaIDs())
				resultData.CharaInTeamName[charaID] = teamName;
		}

		resultData.Ranking = GetRanking();
		GameManager* gameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();
		gameManager->SetGameResult(resultData);
	}
	break;
	}
}

void MatchManager::StatePhaseGameOver(FSMSignal sig)
{
	std::string winner = "";
	int high = 0;
	switch (sig)
	{
	case FSMSignal::SIG_Enter:
	{
		// 勝利チームを求める
		for (int i = 0; i < GAME_REF.TeamNames.size(); i++)
		{
			// 要素1
			Team* team = m_pTeamManager->GetTeam(GAME_REF.TeamNames[i]);

			// 勝利ポイントを超えているか
			if (team->GetTotalPoint() >= m_GameData.m_WinPointMax)
			{
				// 超えてたらこのチームが勝者 ( 現状同時に勝利ポイントを超過する可能性アリ )
				winner = team->GetTeamName();
				break;
			}

			// 超えてなければ現状の最高ポイントを超えているかを見る
			if (team->GetTotalPoint() > high)
			{
				winner = team->GetTeamName();
				high = team->GetTotalPoint();
			}
		}

		m_GameData.m_WinnerTeam = winner;

		break;
	case FSMSignal::SIG_Update:
	{
		m_pFsm->ChangeState(&MatchManager::StatePhaseEnd);
	}
	break;
	case FSMSignal::SIG_AfterUpdate:
	{

	}
	break;
	case FSMSignal::SIG_Exit:
	{

	}
	break;
	}
	}
}

void MatchManager::StatePhaseEnd(FSMSignal sig)
{
	switch (sig)
	{
	case FSMSignal::SIG_Enter:
	{
		// 勝利演出画像を設定
		if (m_UI_Result_Won == nullptr)
		{
			m_UI_Result_Won = new UI_Canvas();
			m_UI_Result_Won->rectTransform->anchor.SetPreset(Anchor::Preset::Middle);
			m_UI_Result_Won->SetTag("UI_Result_Won");
			m_UI_Result_Won->SetImage(LoadGraph("data/texture/UI/Result/YouWon.png"));
			m_UI_Result_Won->SetEasing(UI_Canvas::eAlpha, 0, 255, 1.0f, EasingType::Linear);
			m_UI_Result_Won->SetEasing(UI_Canvas::eScale, Vector2::Zero, Vector2::Ones, 1.0f, EasingType::Linear);
		}

		// 敗北演出画像を設定
		if (m_UI_Result_Lost == nullptr)
		{
			m_UI_Result_Lost = new UI_Canvas();
			m_UI_Result_Lost->rectTransform->anchor.SetPreset(Anchor::Preset::Middle);
			m_UI_Result_Lost->SetTag("UI_Result_Lost");
			m_UI_Result_Lost->SetImage(LoadGraph("data/texture/UI/Result/YouLost.png"));
			m_UI_Result_Lost->SetEasing(UI_Canvas::eAlpha, 0, 255, 1.0f, EasingType::Linear);
			m_UI_Result_Lost->SetEasing(UI_Canvas::eScale, Vector2::Zero, Vector2::Ones, 1.0f, EasingType::Linear);
		}

		// 勝利チームに応じて、描画位置を変化させる(今のところベタ打ち)
		// TO:DO チームと画面分割先を紐づける
		if (m_GameData.m_WinnerTeam == "Red")
		{
			m_UI_Result_Won->rectTransform->anchor.SetBegin(ScreenManager::GetScreenBeginPos(0));
			m_UI_Result_Won->rectTransform->anchor.SetEnd(ScreenManager::GetScreenEndPos(0));

			m_UI_Result_Lost->rectTransform->anchor.SetBegin(ScreenManager::GetScreenBeginPos(1));
			m_UI_Result_Lost->rectTransform->anchor.SetEnd(ScreenManager::GetScreenEndPos(1));
		}
		else
		{
			m_UI_Result_Won->rectTransform->anchor.SetBegin(ScreenManager::GetScreenBeginPos(1));
			m_UI_Result_Won->rectTransform->anchor.SetEnd(ScreenManager::GetScreenEndPos(1));

			m_UI_Result_Lost->rectTransform->anchor.SetBegin(ScreenManager::GetScreenBeginPos(0));
			m_UI_Result_Lost->rectTransform->anchor.SetEnd(ScreenManager::GetScreenEndPos(0));
		}
	}
	break;
	case FSMSignal::SIG_Update:
	{
		// 演出が終了して、ボタンが押された場合
		if (m_UI_Result_Won != nullptr && not m_UI_Result_Won->IsAllEasingRun() && 
			m_UI_Result_Lost != nullptr && not m_UI_Result_Lost->IsAllEasingRun() &&
			InputManager::Push("AnyKey"))
		{
			Fader::FadeStart(1.0f, EasingType::Linear, 0.0f, 255.0f, GameTime::AdditionMethod::Usual);
			m_IsFadeEnd = true;
		}

		if (m_IsFadeEnd && not Fader::IsPlaying())
			SceneManager::ChangeScene("ResultScene");
	}
	break;
	case FSMSignal::SIG_AfterUpdate:
	{

	}
	break;
	case FSMSignal::SIG_Exit:
	{

	}
	break;
	}
}

Chara* MatchManager::addCharacter(const std::string& team, const Transform& trs, bool isAI)
{
	Chara* chara = m_pCharaManager->Create(team, trs);

	chara->SetMoveSpeed(CHARADEFINE_REF.MoveSpeed);
	chara->SetRotSpeed(MathUtil::ToRadians(CHARADEFINE_REF.RotSpeed));

	registerChara(isAI, chara);

	return chara;
}

Chara* MatchManager::addCharacter(const User& user, const std::string& team, const Transform& trs, bool isAI)
{
	Chara* chara = m_pCharaManager->Create(team, trs, user);

	chara->SetMoveSpeed(CHARADEFINE_REF.MoveSpeed);
	chara->SetRotSpeed(MathUtil::ToRadians(CHARADEFINE_REF.RotSpeed));

	registerChara(isAI, chara);
	
	return chara;
}

void MatchManager::registerChara(bool isAI, Chara* chara)
{
	std::unordered_map<int, int> padNumMap =
	{
		{0, DX_INPUT_PAD1},
		{1, DX_INPUT_PAD2},
		{2, DX_INPUT_PAD3},
		{3, DX_INPUT_PAD4},
		{4, DX_INPUT_PAD5},
		{5, DX_INPUT_PAD6},
		{6, DX_INPUT_PAD7},
		{7, DX_INPUT_PAD8},
		{8, DX_INPUT_PAD9},
		{9, DX_INPUT_PAD10},
		{10, DX_INPUT_PAD11},
	};

	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (chara->GetUser().UUID == net.UUID)
		{
			chara->AddComponent<PlayerController>()->Init(padNumMap[0]);
		}
		else
		{
			// ネットワークモードの場合、クライアントと紐づいているキャラ以外はコントローラーが不要なので何もつけない
		}
	}
	else
	{
		if (not isAI)
			chara->AddComponent<PlayerController>()->Init(padNumMap[chara->GetIndex()]);
		else
			chara->AddComponent<AIController>()->Init();
	}

	m_pTeamManager->RegisterCharaToTeam(chara);
}

