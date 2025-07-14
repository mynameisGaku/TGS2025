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
    /// キャラのタグとインデックスをチームに登録
    /// </summary>
    void RegisterCharaToTeam(class Chara* pChara);

    Team* GetTeam(const std::string& name);

    std::list<Team*> GetTeams();

	/// <summary>
	/// キャラのIDからチーム名を取得
	/// </summary>
	const std::string GetTeamName(int charaID) const;

private:
    void init();

    std::list<Team*> m_Teams;
};