#pragma once
#include <vector>
#include <string>
#include <utility>
#include <list>

class Team
{
public:

    /// <summary>
    /// チーム名を設定して初期化
    /// </summary>
    Team(const std::string& name);

    /// <summary>
    /// チームにIDを登録する
    /// </summary>
    void AddCharaID(int id);

    /// <summary>
    /// チームメンバー全体の合算ポイントを取得
    /// </summary>
    int GetTotalPoint();

    /// <summary>
    /// 所属しているキャラ達のIDを取得
    /// </summary>
    std::vector<int> GetCharaIDs();

    /// <summary>
    /// ポイント最多者のIDとポイントをセットで取得
    /// </summary>
    std::pair<int, int> GetHighestPointHolderIDandPoint();

    /// <summary>
    /// チーム内ランキング配列を取得
    /// </summary>
    std::vector<std::pair<int, int>> GetTeamRanking();

    /// <summary>
    /// チーム名を取得
    /// </summary>
    std::string GetTeamName() const;

    /// <summary>
    /// 
    /// </summary>
    bool IsCharaIDInPool(int id);

private:
    friend class TeamManager;

    /// <summary>
    /// すでに登録されたIDか確認する
    /// </summary>
    bool queryIsRegistered(int id);

    class CharaBase* getCharaFromID(int id);

    std::vector<int>                m_CharaIDs;
    std::string                     m_TeamName;
    class CharaManager*             m_pCharaManager;
    std::list<std::pair<int, int>>  m_PointLog;
    std::list<int>                  m_Points;
};