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
    }
}

void TeamManager::AddPoint(const std::string& team)
{
    for (auto& teamData : m_Teams)
    {
        if (teamData->GetTeamName() not_eq team)
            continue;

        teamData->AddPoint(1);
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

void TeamManager::init()
{
    for (auto& name : GAME_REF.TeamNames)
    {
        Team* team = new Team(name);
        m_Teams.push_back(team);
    }
}
