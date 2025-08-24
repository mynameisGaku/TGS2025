#include "UI_GameScore.h"
#include "src/common/camera/CameraManager.h"
#include "src/scene/play/match/MatchManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/screen/ScreenManager.h"

UI_GameScore::UI_GameScore() : UI_GameScore(RectTransform())
{
}

UI_GameScore::UI_GameScore(const RectTransform& trs)
{
	m_pMatchManager = nullptr;
	m_Size = 0;
	m_AchievedScore = -1;
	SetTransform(trs);

	FontInfo fontInfo = Font::BasicFont();
	fontInfo.SetCharSet(DEFAULT_CHARSET).SetSize(32).SetFontType(DX_FONTTYPE_NORMAL);
	m_hFontScore = Font::Create(fontInfo, "Score");
	fontInfo.SetCharSet(DEFAULT_CHARSET).SetSize(16).SetFontType(DX_FONTTYPE_NORMAL);
	m_hFontAchievedScore = Font::Create(fontInfo, "AchievedScore");
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
	RectTransform rectTrs = RectTransform(Anchor::Preset::RightDown);
	rectTrs.position = Vector2(-5.0f, -100.0f);

	if (CameraManager::IsScreenDivision())
	{
		const int CAMERA_NUM = (int)CameraManager::AllCameras().size();


		for (int i = 0; i < CAMERA_NUM; i++)
		{
			Vector2 begin = CameraManager::GetDrawingAreaPos_CameraIndex(i);
			Vector2 size = CameraManager::GetDrawingAreaSize_CameraIndex(i);
			rectTrs.anchor.SetBegin(begin);
			rectTrs.anchor.SetEnd(begin + size);

			drawTotalScore(rectTrs.Global().position);
		}
	}
	else
	{
		drawTotalScore(rectTrs.Global().position);
	}

	//drawUserScores();
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
	m_Size++;
}

void UI_GameScore::drawTotalScore(const Vector2 position)
{
	int index = 0;
	const Vector2 adjust = Vector2(120.0f, 80.0f);
	const float backWidth = adjust.x * m_Size;

	// 背景色を追加
	for (const auto& it : m_BackColors) {

		const Vector2 begin =	Vector2(position.x + adjust.x * index		- backWidth,	position.y - adjust.y * 0.5f);
		const Vector2 end =		Vector2(position.x + adjust.x * (index + 1)	- backWidth,	position.y + adjust.y * 0.0f);

		DrawBoxAA(begin.x, begin.y, end.x, end.y, it.second, true);
		index++;
	}

	index = 0;

	// 全体スコア表示
	for (const auto& it : m_TotalScores) {

		const std::string scoreText = StringUtil::FormatToString("%d", it.second);
		const int width = GetDrawStringWidthToHandle(scoreText.c_str(), (int)scoreText.length(), m_hFontScore);
		const Vector2 pos = Vector2(position.x + adjust.x * index + adjust.x * 0.5f - backWidth - width * 0.5f, position.y - adjust.y * 0.4f);
	
		DrawFormatStringFToHandle(pos.x, pos.y, 0xffffff, m_hFontScore, scoreText.c_str());
		index++;
	}

	// 必要スコア表示
	if (m_AchievedScore > 0)
	{
		const std::string scoreText = StringUtil::FormatToString("必要スコア:%d", m_AchievedScore);
		int width = 0;
		int height = 0;
		GetDrawStringSizeToHandle(&width, &height, nullptr, scoreText.c_str(), (int)scoreText.length(), m_hFontAchievedScore);

		const Vector2 pos	= Vector2(position.x - width * 0.5f - backWidth * 0.5f, position.y - adjust.y * 0.5f - height);
		const Vector2 end	= Vector2(pos.x + width, pos.y + height);

		DrawBoxAA(pos.x, pos.y, end.x, end.y, 0x999999, true);
		DrawFormatStringFToHandle(pos.x, pos.y, 0xffffff, m_hFontAchievedScore, scoreText.c_str());
	}
}

void UI_GameScore::drawUserScores()
{
	if (m_pMatchManager == nullptr)
		return;  // マッチマネージャーが初期化されていない場合は何もしない

	auto ranking = m_pMatchManager->GetRanking();
	const int dispCount = 4;  // 表示するランキングの数
	int index = 0;

	for (const auto& rank : ranking)
	{
		Camera* camera = CameraManager::GetCameraDrawing();
		if (camera == nullptr)
			return;

		Vector2 drawAreaPos = Vector2::Zero;
		Vector2 drawAreaSize = Vector2::Zero;
		Vector2 drawAreaEnd = Vector2::Zero;
		camera->GetUsingDrawArea(&drawAreaPos, &drawAreaSize);

		drawAreaEnd = drawAreaPos + drawAreaSize;

		RectTransform rectTrs = RectTransform(Anchor::Preset::RightDown, Vector2(-250, -GetFontSize() * dispCount));
		rectTrs.anchor.SetBegin(drawAreaPos);
		rectTrs.anchor.SetEnd(drawAreaEnd);

		const Vector2 size = Vector2(250, GetFontSize());
		const std::string teamName = m_pMatchManager->GetTeamName(rank.first);
		const int color = m_BackColors.contains(teamName) ? m_BackColors[teamName] : 0xFFFFFF;  // 背景色を取得、なければ黒

		DrawBoxAA(
			rectTrs.Global().position.x,
			rectTrs.Global().position.y + size.y * index,
			rectTrs.Global().position.x + size.x,
			rectTrs.Global().position.y + size.y * (index + 1),
			color, true);

		DrawFormatString((int)(rectTrs.Global().position.x), (int)(rectTrs.Global().position.y + index * GetFontSize()), GetColor(255, 255, 255), "ID %d:Score %2d", rank.first, rank.second);

		if (++index >= dispCount)
			break;  // 表示するランキングの数を制限
	}
}
