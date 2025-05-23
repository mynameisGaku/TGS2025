#pragma once
#include <vector>
#include "framework/gameObject.h"
#include "src/common/game/GameManager.h"

class CurrentGameData
{
public:
    std::string         m_Name;
    int                 m_WinPointMax;
    std::vector<int>    m_TeamPoints;
    float               m_PlayTimeMaxSec;
    float               m_PlayTimeCounterSec;
public:
    CurrentGameData(const GAME_MODE_DESC& desc = {});
};

class MatchManager : public GameObject
{
public:
    MatchManager();
    ~MatchManager();

    void Update() override;
    void Draw() override;
private:
    
    void init();

    CurrentGameData m_GameData;
};