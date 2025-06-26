#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>
#include <src/util/debug/imgui/imGuiManager.h>

//=== キャラ ===
#include "src/util/math/MathUtil.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"
#include "src/scene/play/status_tracker/StatusTracker.h"

//=== チーム ===
#include "src/scene/play/team/Team.h"
#include "src/scene/play/team/TeamManager.h"

//=== ボール ===
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include <src/util/string/StringUtil.h>

//=== カメラ ===
#include "src/common/camera/CameraManager.h"

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
    if (not CameraManager::IsScreenDivision())
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
    ImGuiRoot* matchRoot = ImGuiManager::AddRoot(new ImGuiRoot("Match"));  // マッチのトップ

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

        addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);
        addCharacter("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);

        // 追加できるの確認したよ
        //addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 150.0f), Vector3::Zero, Vector3::Ones), false);
        //addCharacter("Blue", Transform(Vector3(250.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);
        //addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 250.0f), Vector3::Zero, Vector3::Ones), false);
        //addCharacter("Blue", Transform(Vector3(350.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);

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

    chara->SetMoveSpeed(CHARADEFINE_REF.MoveSpeed);
    chara->SetRotSpeed(MathUtil::ToRadians(CHARADEFINE_REF.RotSpeed));

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

