#include "src/common/game/GameManager.h"

GameManager::GameManager()
{
    init();
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
}