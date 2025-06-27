#include "UI_GameScore.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/match/MatchManager.h"

UI_GameScore::UI_GameScore() : UI_GameScore(RectTransform())
{
}

UI_GameScore::UI_GameScore(const RectTransform& trs)
{
	m_pMatchManager = nullptr;
	size = 0;
	SetTransform(trs);
}

UI_GameScore::~UI_GameScore()
{
}

void UI_GameScore::Update()
{
	for (auto& score : m_TotalScores)
		score.second = 0;  // スコアをリセット

	for (const auto& score : m_UserScores)
	{
		m_TotalScores[score.second.first] += score.second.second;  // ユーザースコアを集計
	}
}

void UI_GameScore::Draw()
{
	int index = 0;
	const float adjust = 90.0f;
	const RectTransform globalTrs = rectTransform->Global();

	for (const auto& it : m_BackColors) {

		DrawBoxAA(
			globalTrs.position.x - adjust + (adjust * index),
			globalTrs.position.y - adjust * 0.5f,
			globalTrs.position.x + adjust * index,
			globalTrs.position.y + adjust * 0.5f,
			it.second, true);

		index++;
	}

	index = 0;

	for (const auto& it : m_TotalScores) {
		DrawFormatStringF(
			globalTrs.position.x - adjust + (adjust * index),
			globalTrs.position.y + adjust * 0.1f,
			0xFFFFFF, "%3d",
			it.second);

		index++;
	}

	if (m_pMatchManager == nullptr)
		return;  // マッチマネージャーが初期化されていない場合は何もしない

	auto ranking = m_pMatchManager->GetRanking();
	const int dispCount = 4;  // 表示するランキングの数
	index = 0;

	for (const auto& rank : ranking)
	{
		Vector2 beginPos = CameraManager::GetScreenDivisionPos();
		Vector2 endPos = CameraManager::GetScreenDivisionSize();

		RectTransform rectTrs = RectTransform(Anchor::Preset::RightDown, Vector2(-250, -GetFontSize() * dispCount));
		rectTrs.anchor.SetBegin(beginPos);
		rectTrs.anchor.SetEnd(beginPos + endPos);

		const Vector2 size = Vector2(250, GetFontSize());
		const std::string teamName = m_pMatchManager->GetTeamName(rank.first);
		const int color = m_BackColors.contains(teamName) ? m_BackColors[teamName] : 0xFFFFFF;  // 背景色を取得、なければ黒

		DrawBoxAA(
			rectTrs.Global().position.x,
			rectTrs.Global().position.y + size.y * index,
			rectTrs.Global().position.x + size.x,
			rectTrs.Global().position.y + size.y * (index + 1),
			color, true);

		DrawFormatString(rectTrs.Global().position.x, rectTrs.Global().position.y + index * GetFontSize(), GetColor(255, 255, 255), "ID %d:Score %2d", rank.first, rank.second);

		if (++index >= dispCount)
			break;  // 表示するランキングの数を制限
	}
}

void UI_GameScore::SetUserScore(const std::string& teamName, int id, int score)
{
	m_UserScores[id].first = teamName;
	m_UserScores[id].second = score;

	if (m_TotalScores.contains(teamName))
	{
		if (m_TotalScores[teamName] < score)
			m_TotalScores[teamName] = score;
	}
}

void UI_GameScore::AddUserScore(const std::string& teamName, int id, int score, int backColor)
{
	m_UserScores[id].first = teamName;
	m_UserScores[id].second = score;

	if (m_TotalScores.contains(teamName))
	{
		if (m_TotalScores[teamName] < score)
			m_TotalScores[teamName] = score;
	}

	m_BackColors[teamName] = backColor;
	size++;
}
