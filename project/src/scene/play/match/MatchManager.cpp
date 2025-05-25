#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>
#include <src/util/debug/imgui/imGuiManager.h>

//=== キャラ ===
#include "src/util/math/mathUtils.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"

//=== チーム ===
#include "src/scene/play/team/Team.h"
#include "src/scene/play/team/TeamManager.h"

//=== ボール ===
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include <src/util/string/StringUtil.h>


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
}

void MatchManager::Update()
{
    m_pFsm->Update();
}

void MatchManager::Draw()
{
    m_pFsm->ImGuiDebugRender();
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
}

void MatchManager::ImGuiInit()
{
#ifdef _DEBUG
    ImGuiRoot* MatchTree = ImGuiManager::AddRoot(new ImGuiRoot("Match"));	// マッチのツリー

    ImGuiRoot* TeamsTree = MatchTree->AddChild(new ImGuiRoot("Teams"));	// カメラの情報を閲覧するツリー
    for (auto& teamData : m_pTeamManager->GetTeams())
    {
        ImGuiRoot* TeamBranch = TeamsTree->AddChild(new ImGuiRoot(teamData->GetTeamName() + " " + "Team"));	// カメラの情報を閲覧するツリー

        TeamBranch->Add(new ImGuiNode_Text("charaIDs", StringUtil::FormatToString("Chara ID")));
        TeamBranch->NodeBeginChild(250.0f, 60.0f);
        for (auto& id : teamData->GetCharaIDs())
        {
            TeamBranch->Add(new ImGuiNode_Text("id", StringUtil::FormatToString("id:%d", id)));
        }
        TeamBranch->NodeEndChild();

        TeamBranch->Add(new ImGuiNode_Text("ranking", StringUtil::FormatToString("Ranking")));
        TeamBranch->Add(new ImGuiNode_Text("totalPoint", StringUtil::FormatToString("TotalPoint:%d", teamData->GetTotalPoint())));
        TeamBranch->NodeBeginChild(250.0f, 60.0f);
        size_t i = 0;
        for (auto& rankData : teamData->GetTeamRanking())
        {
            i++;
            TeamBranch->Add(new ImGuiNode_Text("rankData", StringUtil::FormatToString("rank:%d [ id:%d | point:%d]", i, rankData.first, rankData.second)));
        }
        TeamBranch->NodeEndChild();
    }

#endif // _DEBUG
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

        addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);
        addCharacter("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);

        m_pBallManager = Instantiate<BallManager>();

        // 現在のゲームモードとそのデータを取得し、取っておく
        m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());

        ImGuiInit();
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

    }
    break;
    }
}

void MatchManager::StatePhaseGameOver(FSMSignal sig)
{
    switch (sig)
    {
    case FSMSignal::SIG_Enter:
    {

    }
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

void MatchManager::StatePhaseEnd(FSMSignal sig)
{
    switch (sig)
    {
    case FSMSignal::SIG_Enter:
    {
        SceneManager::ChangeScene("TitleScene");
    }
    break;
    case FSMSignal::SIG_Update:
    {

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

void MatchManager::addCharacter(const std::string& team, const Transform& trs, bool isAI)
{
    CharaBase* chara = m_pCharaManager->Create(team, trs);

    chara->SetMoveSpeed(700.0f);
    chara->SetRotSpeed(MathUtil::ToRadians(10.0f));

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

    if (not isAI)
        chara->AddComponent<PlayerController>()->Init(padNumMap[chara->GetIndex()]);
    else
        chara->AddComponent<AIController>()->Init();

    m_pTeamManager->RegisterCharaToTeam(chara);
}

