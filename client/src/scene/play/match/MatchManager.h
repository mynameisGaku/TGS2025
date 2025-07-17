#pragma once
#include <vector>
#include "framework/gameObject.h"
#include "src/common/game/GameManager.h"
#include "src/util/fsm/TinyFSM.h"
#include <src/util/transform/Transform.h>
#include <src/common/network/user/User.h>

class CurrentGameData
{
public:
    std::string                             m_Name{};
    int                                     m_WinPointMax{};
    std::string                             m_WinnerTeam{};
    std::unordered_map<std::string, int>    m_TeamPointMap{};
    float                                   m_PlayTimeMaxSec{};
    float                                   m_PlayTimeCounterSec{};
public:
    CurrentGameData(const GAME_MODE_DESC& desc = {}, std::vector<std::string> teamNames = {});
};

class UI_Canvas;
class UI_GameScore;

class MatchManager : public GameObject
{
public:
    MatchManager();
    ~MatchManager();

    void Update() override;
    void Draw() override;

    void ReloadCurrentGameData();

    std::string     GetWinnerTeamName();
    float           GetPlayTimeSec();
    float           GetPlayTimeMaxSec();
    int             GetWinPointMax();
    float           GetReadyTimerSec();

	std::vector<std::pair<int, int>> GetRanking() const;
	const std::string GetTeamName(int charaID) const;

private:

    void init();

    /* 以下ステート */

    /* Begin */
    void StatePhaseBegin(FSMSignal sig);

    void ImGuiInit();

    /* Ready */
    void StatePhaseReady(FSMSignal sig);
    float m_ReadyEndCounterSec;

    /* Play */
    void StatePhasePlay(FSMSignal sig);
    float m_PlayTimeCounter;

    /* GameOver */
    void StatePhaseGameOver(FSMSignal sig);
    float m_GameOverEndCounterSec;

    /* End */
    void StatePhaseEnd(FSMSignal sig);

    CurrentGameData m_GameData;
    TinyFSM<MatchManager>* m_pFsm;

    class CharaManager* m_pCharaManager;
    class BallManager* m_pBallManager;
    class TeamManager* m_pTeamManager;

    UI_GameScore* m_UI_GameScore;
	UI_Canvas* m_UI_Result_Won;     // 勝利演出画像
	UI_Canvas* m_UI_Result_Lost;    // 敗北演出画像
    float m_GameEndTime;
    bool m_IsFadeEnd;

    Chara* addCharacter(const std::string& team, const Transform& trs, bool isAI);
    Chara* addCharacter(const User& user, const std::string& team, const Transform& trs, bool isAI);

    void registerChara(bool isAI, Chara* chara);

};