#pragma once
#include <vector>
#include <utility>

class Team
{
public:

    /// <summary>
    /// チームにIDを登録する
    /// </summary>
    void AddCharaID(int id);

    /// <summary>
    /// チームメンバー全体の合算ポイントを取得
    /// </summary>
    int GetTotalPoint();

    /// <summary>
    /// ポイント最多者のIDとポイントをセットで取得
    /// </summary>
    std::pair<int, int> GetHighestPointHolderIDandPoint();

    /// <summary>
    /// チーム内ランキング配列を取得
    /// </summary>
    std::vector<std::pair<int, int>> GetTeamRanking();

private:

    /// <summary>
    /// すでに登録されたIDか確認する
    /// </summary>
    bool queryIsRegistered(int id);

    /// <summary>
    /// 
    /// </summary>
    bool isCharaIDInPool(int id);

    class CharaBase* getCharaFromID(int id);

    std::vector<int> m_CharaIDs;

    class CharaManager* m_pCharaManager;
};