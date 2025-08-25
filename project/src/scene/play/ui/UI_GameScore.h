#pragma once
#include "src/util/ui/UI_Canvas.h"
#include <unordered_map>
#include <vector>
#include <src/util/font/Font.h>

class MatchManager;
class SliceBar;

class UI_GameScore : public UI_Canvas {
public:
	UI_GameScore();
	UI_GameScore(const RectTransform& trs);
	~UI_GameScore();

	void Update() override;
	void Draw() override;

	inline void SetMatchManager(const MatchManager* matchManager) { m_pMatchManager = matchManager; }
	inline void SetAchievedScore(int score) { m_AchievedScore = score; }

	void SetUserScore(const std::string& teamName, int id, int score);
	void AddUserScore(const std::string& teamName, int id, int score, int backColor);

private:

	void drawTotalScore(const Vector2 position);
	void drawUserScores();

	std::unordered_map<std::string, int> m_TotalScores;
	std::unordered_map<std::string, int> m_BackColors;

	std::unordered_map<int, std::pair<std::string, int>> m_UserScores;
	int m_Size;
	int m_AchievedScore;

	const MatchManager* m_pMatchManager;

	int m_hFontScore{};
	int m_hFontAchievedScore{};

	int m_hBarImage;
	SliceBar* m_BackBar;
};