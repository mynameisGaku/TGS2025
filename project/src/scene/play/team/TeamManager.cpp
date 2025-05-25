#include "TeamManager.h"
#include "src/reference/game/GameRef.h"
#include "src/scene//play/chara/CharaBase.h"

TeamManager::TeamManager()
{
    init();
}

TeamManager::~TeamManager()
{
}

void TeamManager::RegisterCharaToTeam(CharaBase* pChara)
{
    for (auto& teamData : m_Teams)
    {
        if (teamData->GetTeamName() not_eq pChara->GetCharaTag())
            continue;

        teamData->AddCharaID(pChara->GetIndex());
        return;
    }
}

Team* TeamManager::GetTeam(const std::string& name)
{
    for (auto& teamData : m_Teams)
    {
        if (teamData->GetTeamName() == name)
            return teamData;
    }
    return nullptr;
}

std::list<Team*> TeamManager::GetTeams()
{
    return m_Teams;
}

void TeamManager::init()
{
    for (auto& name : GAME_REF.TeamNames)
    {
        Team* team = new Team(name);
        m_Teams.push_back(team);
    }
}
