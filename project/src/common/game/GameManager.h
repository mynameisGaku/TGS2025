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

    void Update() override;
    void DrawImGuiDebugger();

    void SetGameModeName(const std::string& name) { m_CurrentGameMode = name; }

    GAME_MODE_DESC GetCurrentGameModeData();

    struct ResultData
    {
		std::vector<std::string> WinnerTeamName;
        std::vector<int> MVP_CharaIndex;
        std::vector<int> TotalPoint;
    };

    void SetGameResult(const ResultData& data);

    std::string GetCurrentStageName() const;

    inline ResultData GetResultData() const { return m_ResultData; }
	inline ResultData GetResultDataPrev() const { return m_ResultDataPrev; }

private:

    void init();

    // 今後使いそう
    //std::list<class UserData*> m_UserDatas;

    std::unordered_map<std::string, GAME_MODE_DESC> m_GameModeDescs;
    std::vector<std::string>                        m_GameModeNames;
    std::vector<std::string>                        m_TeamNames;
    std::vector<std::string>                        m_StageNames;

    std::string                                     m_CurrentStageName;
    std::string                                     m_CurrentGameMode;
    ResultData                                      m_ResultData;
    ResultData                                      m_ResultDataPrev;
};