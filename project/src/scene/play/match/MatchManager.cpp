#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>

//=== キャラ ===
#include "src/util/math/mathUtils.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/common/component/controller/PlayerController.h"
#include "src/common/component/controller/AIController.h"
#include "src/common/component/controller/DebugController.h"

//=== ボール ===
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

/* 以下ステート */

void MatchManager::StatePhaseBegin(FSMSignal sig)
{
    switch (sig)
    {
    case FSMSignal::SIG_Enter:
    {
        /* ここで準備を行う */

        m_pCharaManager = Instantiate<CharaManager>();

        CharaBase* player = m_pCharaManager->Create("Red", Transform(Vector3(0.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones));
        CharaBase* enemy = m_pCharaManager->Create("Blue", Transform(Vector3(150.0f, 0.0f, 0.0f), Vector3::Zero, Vector3::Ones));

        player->SetMoveSpeed(700.0f);
        player->SetRotSpeed(MathUtil::ToRadians(10.0f));
        player->AddComponent<PlayerController>()->Init(DX_INPUT_PAD1);

        enemy->SetMoveSpeed(700.0f);
        enemy->SetRotSpeed(MathUtil::ToRadians(10.0f));

        // デバッグによってコントローラーを変える。
#if FALSE
        enemy->AddComponent<AIController>()->Init();
#elif FALSE
        enemy->AddComponent<DebugController>()->Init(DX_INPUT_PAD1);
#elif TRUE
        enemy->AddComponent<PlayerController>()->Init(DX_INPUT_PAD2);
#endif

        m_pBallManager = Instantiate<BallManager>();

        // 現在のゲームモードとそのデータを取得し、取っておく
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
