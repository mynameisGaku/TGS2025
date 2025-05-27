#include "src/common/game/GameManager.h"

GameManager::GameManager()
{
    init();
}

GameManager::~GameManager()
{
}

GAME_MODE_DESC GameManager::GetCurrentGameModeData()
{
    return m_GameModeDescs[m_CurrentGameMode];
}

void GameManager::SetGameResult(const ResultData& data)
{
    m_ResultData = data;
}

void GameManager::init()
{
    GAME_REF.Load();

    m_GameModeDescs = GAME_REF.GameModeDescs;
    m_GameModeNames = GAME_REF.GameModeNames;
    m_TeamNames     = GAME_REF.TeamNames;
}