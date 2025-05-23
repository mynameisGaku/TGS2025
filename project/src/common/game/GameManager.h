#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "framework/gameObject.h"
#include "src/util/fsm/TinyFSM.h"
#include "src/reference/game/GameRef.h"

/// <summary>
/// ゲームの進行を管理するマネージャー
/// </summary>
class GameManager : public GameObject
{
public:
    GameManager();
    ~GameManager();

    void SetGameModeName(const std::string& name) { m_CurrentGameMode = name; }

    GAME_MODE_DESC GetCurrentGameModeData();

private:

    void init();

    // 今後使いそう
    //std::list<class UserData*> m_UserDatas;

    std::unordered_map<std::string, GAME_MODE_DESC> m_GameModeDescs;
    std::vector<std::string>                        m_GameModeNames;
    std::vector<std::string>                        m_TeamNames;

    std::string                                     m_CurrentGameMode;
    std::string                                     m_WinnerTeamName;
};