#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include <framework/gameObject.h>
#include <src/util/fsm/TinyFSM.h>
#include <src/reference/game/GameRef.h>

class Chara;

/// <summary>
/// ゲームの進行を管理するマネージャー
/// </summary>
class GameManager : public GameObject
{
public:
	GameManager();
	~GameManager();

	void Update() override;
	void DrawImGuiDebugger();

	void SetGameModeName(const std::string& name) { m_CurrentGameMode = name; }

	GAME_MODE_DESC GetCurrentGameModeData();

	struct ResultData
	{
		std::vector<std::string> WinnerTeamName;// 勝利チームの名前
		std::vector<int> WinnerCharaIDs;        // チームごとのキャラID

		std::vector<std::string> TeamName;                   // 各種チームの名前
		std::unordered_map<std::string, int> TeamColor;      // 各種チームのカラー
		std::unordered_map<std::string, int> TeamTotalPoint; // 各種チームの総合得点

		std::vector<std::pair<int, int>> Ranking;            // ランキングデータ。 [順位].first = キャラID、[順位].second = スコア
		std::unordered_map<int, std::string> CharaInTeamName;// キャラIDと所属しているチームの紐づけ
	};

	void SetGameResult(const ResultData& data);

	std::string GetCurrentStageName() const;

	inline ResultData GetResultData() const { return m_ResultData; }
	inline ResultData GetResultDataPrev() const { return m_ResultDataPrev; }

private:

	void init();

	std::unordered_map<std::string, GAME_MODE_DESC>	m_GameModeDescs;
	std::vector<std::string>						m_GameModeNames;
	std::vector<std::string>						m_TeamNames;
	std::vector<std::string>						m_StageNames;

	std::string										m_CurrentStageName;
	std::string										m_CurrentGameMode;
	ResultData										m_ResultData;
	ResultData										m_ResultDataPrev;
};