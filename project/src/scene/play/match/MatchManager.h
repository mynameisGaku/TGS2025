#pragma once
#include <vector>
#include "framework/gameObject.h"
#include "src/common/game/GameManager.h"
#include "src/util/fsm/TinyFSM.h"

class CurrentGameData
{
public:
    std::string                             m_Name;
    int                                     m_WinPointMax;
    std::unordered_map<std::string, int>    m_TeamPointMap;
    float                                   m_PlayTimeMaxSec;
    float                                   m_PlayTimeCounterSec;
public:
    CurrentGameData(const GAME_MODE_DESC& desc = {}, std::vector<std::string> teamNames = {});
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

    /* 以下ステート */

    /* Begin */
    void StatePhaseBegin(FSMSignal sig);

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

};