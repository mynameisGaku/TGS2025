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

    void AddPoint(const std::string& team);

    Team* GetTeam(const std::string& name);

private:
    void init();

    std::list<Team*> m_Teams;
};