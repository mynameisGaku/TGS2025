#include "MatchManager.h"
#include "src/common/game/GameManager.h"

CurrentGameData::CurrentGameData(const GAME_MODE_DESC& desc)
{
    m_Name = desc.GameModeName;
    m_WinPointMax = desc.WinPointMax;
    m_PlayTimeMaxSec = desc.PlayTimeMaxSec;
    m_PlayTimeCounterSec = 0.0f;
    m_TeamPoints = { 0 };
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
}

void MatchManager::Draw()
{
}

void MatchManager::init()
{
    m_GameData = CurrentGameData(SceneManager::CommonScene()->FindGameObject<GameManager>()->GetCurrentGameModeData());
}
