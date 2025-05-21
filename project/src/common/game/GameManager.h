#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "framework/gameObject.h"
#include "src/util/fsm/TinyFSM.h"
#include "src/reference/game/GameRef.h"

class GameManager : public GameObject
{
public:
    GameManager();
    ~GameManager();

    void Update() override;
    void Draw() override;

private:

    void init();

    void StateBegin(FSMSignal sig);
    void StateReady(FSMSignal sig);
    void StatePlay(FSMSignal sig);
    void StateGameOver(FSMSignal sig);
    void StateEnd(FSMSignal sig);

    void StateDrawBegin(FSMSignal sig);
    void StateDrawReady(FSMSignal sig);
    void StateDrawPlay(FSMSignal sig);
    void StateDrawGameOver(FSMSignal sig);
    void StateDrawEnd(FSMSignal sig);

    TinyFSM<GameManager>* m_pFsm;
    TinyFSM<GameManager>* m_pFsmDraw;

    std::unordered_map<std::string, GAME_MODE_DESC> m_GameModeDescs;
    std::vector<std::string>                        m_GameModeNames;
    std::vector<std::string>                        m_TeamNames;
};