#pragma once
#include <list>
#include "framework/gameObject.h"
#include "src/scene/play/team/Team.h"

class TeamManager : public GameObject
{
public:
    TeamManager();
    ~TeamManager();

    /// <summary>
    /// �L�����̃^�O�ƃC���f�b�N�X���`�[���ɓo�^
    /// </summary>
    void RegisterCharaToTeam(class CharaBase* pChara);

    Team* GetTeam(const std::string& name);

    std::list<Team*> GetTeams();

private:
    void init();

    std::list<Team*> m_Teams;
};