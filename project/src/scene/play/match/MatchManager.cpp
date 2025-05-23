#include "MatchManager.h"
#include "src/common/game/GameManager.h"
#include <src/util/time/GameTime.h>

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
}

void MatchManager::init()
{
    m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());

    m_pFsm = new TinyFSM<MatchManager>(this);
    m_pFsm->ChangeState(&MatchManager::StatePhaseBegin);
}

/* 以下ステート */

void MatchManager::StatePhaseBegin(FSMSignal sig)
{
    switch (sig)
    {
    case FSMSignal::SIG_Enter:
    {
        /* ここで準備を行う */

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
