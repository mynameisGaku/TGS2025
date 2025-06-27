#pragma once
#include "src/util/ui/UI_Canvas.h"
#include <unordered_map>
#include <vector>

class MatchManager;

class UI_GameScore : public UI_Canvas {
public:
	UI_GameScore();
	UI_GameScore(const RectTransform& trs);
	~UI_GameScore();

	void Update() override;
	void Draw() override;

	inline void SetMatchManager(const MatchManager* matchManager) { m_pMatchManager = matchManager; }

	void SetUserScore(const std::string& teamName, int id, int score);
	void AddUserScore(const std::string& teamName, int id, int score, int backColor);

private:
	std::unordered_map<std::string, int> m_TotalScores;
	std::unordered_map<std::string, int> m_BackColors;

	std::unordered_map<int, std::pair<std::string, int>> m_UserScores;
	int size;

	const MatchManager* m_pMatchManager;
};