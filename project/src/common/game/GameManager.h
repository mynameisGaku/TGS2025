#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "framework/gameObject.h"
#include "src/util/fsm/TinyFSM.h"
#include "src/reference/game/GameRef.h"

/// <summary>
/// �Q�[���̐i�s���Ǘ�����}�l�[�W���[
/// </summary>
class GameManager : public GameObject
{
public:
    GameManager();
    ~GameManager();

    void Update() override;
    void Draw() override;

private:

    void init();

    // ����g������
    //std::list<class UserData*> m_UserDatas;

    std::unordered_map<std::string, GAME_MODE_DESC> m_GameModeDescs;
    std::vector<std::string>                        m_GameModeNames;
    std::vector<std::string>                        m_TeamNames;

    std::string                                     m_CurrentGameMode;

};