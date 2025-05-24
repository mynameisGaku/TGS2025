#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>

//=== �L���� ===
#include "src/util/math/mathUtils.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/scene/play/chara/CharaHP.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"

//=== �`�[�� ===
#include "src/scene/play/team/Team.h"
#include "src/scene/play/team/TeamManager.h"

//=== �{�[�� ===
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"

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

MatchManager::MatchManager()
{
    init();
}

MatchManager::~MatchManager()
{

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

/* �ȉ��X�e�[�g */

void MatchManager::StatePhaseBegin(FSMSignal sig)
{
    switch (sig)
    {
    case FSMSignal::SIG_Enter:
    {
        /* �����ŏ������s�� */
        m_pCharaManager = Instantiate<CharaManager>();

        m_pTeamManager = Instantiate<TeamManager>();

        addCharacter("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);
        addCharacter("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones), false);

        m_pBallManager = Instantiate<BallManager>();

        // ���݂̃Q�[�����[�h�Ƃ��̃f�[�^���擾���A����Ă���
        m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());
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
        for (auto& item : m_pCharaManager->GetCharaPool()->GetAllItems())
        {
            if (not item->m_IsActive)
                continue;
            auto chara = item->m_pObject;

            // ���͎��񂾂��ǂ����Ń|�C���g�����Z���Ă��܂����A�{���͂����ł͍s�킸�A
            // TeamManager���ŁA�`�[���ɏ������Ă���L�����̃L�������Q�Ƃ��A�����𑫂����킹�����̂�
            // �|�C���g�Ƃ��Ĉ����܂��B�Ȃ̂ŁA���̃��[�v�͏����I�ɏ��ł��܂�
            if (chara->GetHP()->IsDead())
            {
                m_pTeamManager->AddPoint(chara->HitBall()->GetCharaTag());
            }
        }

        for (auto& teamName : GAME_REF.TeamNames)
        {
            if (m_pTeamManager->GetTeam(teamName)->GetPoint() < m_GameData.m_WinPointMax)
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

