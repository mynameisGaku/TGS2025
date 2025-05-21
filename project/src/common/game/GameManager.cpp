#include "src/common/game/GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
}

void GameManager::Draw()
{
}

void GameManager::init()
{
    GAME_REF.Load();

    m_GameModeDescs = GAME_REF.GameModeDescs;
    m_GameModeNames = GAME_REF.GameModeNames;
    m_TeamNames     = GAME_REF.TeamNames;

    m_pFsm = new TinyFSM<GameManager>(this);
    m_pFsmDraw = new TinyFSM<GameManager>(this);

    m_pFsm->ChangeState(&GameManager::StateBegin);
}

void GameManager::StateBegin(FSMSignal sig)
{
}

void GameManager::StateReady(FSMSignal sig)
{
}

void GameManager::StatePlay(FSMSignal sig)
{
}

void GameManager::StateGameOver(FSMSignal sig)
{
}

void GameManager::StateEnd(FSMSignal sig)
{
}
