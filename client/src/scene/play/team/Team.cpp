#include "src/util/math/MathUtil.h"
#include "src/util/time/GameTime.h"
#include "src/scene/play/chara/CharaManager.h"
#include "src/scene/play/status_tracker/StatusTracker.h"
#include "src/scene/play/team/Team.h"
#include <algorithm>

Team::Team(const std::string& name)
{
    m_CharaIDs = {};
    m_TeamName = name;
    m_pCharaManager = FindGameObject<CharaManager>();
}

void Team::AddCharaID(int id)
{
    // 重複回避
    if (queryIsRegistered(id))
        return;

    m_CharaIDs.push_back(id);
}

int Team::GetTotalPoint()
{
    int result = 0;
    for (auto& id : m_CharaIDs)
    {
        if (not IsCharaIDInPool(id))
            continue;

        auto chara = getCharaFromID(id);
        if (not chara)
            continue;

        // キル数に置き換える
        result += chara->GetStatusTracker()->Get_KillCount();
    }

    return result;
}

std::vector<int> Team::GetCharaIDs()
{
    return m_CharaIDs;
}

std::pair<int, int> Team::GetHighestPointHolderIDandPoint()  
{  
   int highestPoint = 0;  
   int highestPointHolderID = -1;  

   for (auto& id : m_CharaIDs)  
   {  
       if (!IsCharaIDInPool(id))
           continue;  

       auto chara = getCharaFromID(id);  
       if (!chara)  
           continue;  

       int charaPoints = 1; // 現在の仕様ではキル数を1ポイントとしてカウント  
       if (charaPoints > highestPoint)  
       {  
           highestPoint = charaPoints;  
           highestPointHolderID = id;  
       }  
   }  

   return {highestPointHolderID, highestPoint};  
}

std::vector<std::pair<int, int>> Team::GetTeamRanking()  
{  
   std::vector<std::pair<int, int>> ranking;  

   for (auto& id : m_CharaIDs)  
   {  
       if (!IsCharaIDInPool(id))
           continue;  

       auto chara = getCharaFromID(id);  
       if (!chara)  
           continue;  

       int charaPoints = chara->GetStatusTracker()->Get_KillCount();
       ranking.emplace_back(id, charaPoints);  
   }  

   // ポイントの降順でソート  
   std::sort(ranking.begin(), ranking.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {  
       return a.second > b.second;  
   });  

   return ranking;  
}

std::string Team::GetTeamName() const
{
    return m_TeamName;
}

bool Team::queryIsRegistered(int _id)
{
    if (m_CharaIDs.size() == 0)
        return false;

    for (auto& id : m_CharaIDs)
    {
        if (id == _id)
            return true;
    }

    return false;
}

bool Team::IsCharaIDInPool(int id)  
{  
  auto charaPool = m_pCharaManager->GetCharaPool();  
  return charaPool->QueryWasRegister(id);  
}

CharaBase* Team::getCharaFromID(int id)
{
    Pool<CharaBase>* charaPool = m_pCharaManager->GetCharaPool();
    return charaPool->Get(id);
}
