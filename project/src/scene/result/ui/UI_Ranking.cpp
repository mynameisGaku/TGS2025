#include "UI_Ranking.h"
#include "src/util/ui/UI_Text.h"
#include "src/util/ui/UI_Manager.h"

#include "src/util/screen/ScreenManager.h"

#include "src/common/game/GameManager.h"
#include "src/common/camera/CameraManager.h"

static const int DISP_MAX = 5;

UI_Ranking::UI_Ranking()
{
	m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();

	const GameManager::ResultData resultData = m_pGameManager->GetResultData();
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();
	Vector2 pos = rectTransform->Global().position;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// リザルトデータに勝者がいる場合
	if (not resultData.Ranking.empty())
	{
		for (int j = 0; j < DISP_MAX; j++)
		{
			if (j >= resultData.Ranking.size()) break;

			int charaId = resultData.Ranking[j].first;
			int score = resultData.Ranking[j].second;

			const std::string text = ("No." + std::to_string(charaId) + " :Score." + std::to_string(score));
			const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
			const std::string teamName = resultData.CharaInTeamName.at(charaId);
			const int color = resultData.TeamColor.at(teamName);
			
			FontInfo font = Font::BasicFont();
			font.SetSize(32).SetColor(color).SetFontType(DX_FONTTYPE_ANTIALIASING_4X4);
			UI_Text* ui_score = new UI_Text(text, RectTransform(), font);
			UI_Manager::Detach(ui_score);
			m_UI_Scores.push_back(ui_score);
		}
	}

	m_CurrentState = STATE::S_NONE;
	m_PrevState = STATE::S_NONE;
}

UI_Ranking::~UI_Ranking()
{
	for (auto it : m_UI_Scores) {
		delete it;
		it = nullptr;
	}

	m_UI_Scores.clear();
}

void UI_Ranking::Update()
{
	switch (m_CurrentState)
	{
	case UI_Ranking::STATE::S_FADE_IN:
		if (not IsAllEasingRun())
			ChangeState(STATE::S_DISPLAY);
		break;

	case UI_Ranking::STATE::S_DISPLAY:
		break;

	default:
		break;
	}

	UI_Canvas::Update();
}

void UI_Ranking::Draw()
{
	UI_Canvas::Draw();

	const GameManager::ResultData resultData = m_pGameManager->GetResultData();
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	// リザルトデータに勝者がいる場合
	if (not resultData.Ranking.empty())
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = CameraManager::GetDrawingAreaPos_CameraIndex(i);
			Vector2 screenEnd = screenBegin + CameraManager::GetDrawingAreaSize_CameraIndex(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			// 最大表示数
			const int DISP_MAX = 5;

			for (int j = 0; j < DISP_MAX; j++)
			{
				if (j >= m_UI_Scores.size()) break;

				int charaId = resultData.Ranking[j].first;
				int score = resultData.Ranking[j].second;

				const std::string text = ("No." + std::to_string(charaId) + " :Score." + std::to_string(score));
				const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
				const std::string teamName = resultData.CharaInTeamName.at(charaId);
				const int color = resultData.TeamColor.at(teamName);

				const Vector2 base = Vector2(screenCenter.x - width * 0.5f, screenCenter.y + 150.0f);
				const Vector2 offset = Vector2(0.0f, 30.0f * j);

				UI_Text* ui_score = m_UI_Scores[j];
				ui_score->rectTransform->position = base + offset;
				ui_score->Draw();
			}
		}
	}
	else
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = CameraManager::GetDrawingAreaPos_CameraIndex(i);
			Vector2 screenEnd = screenBegin + CameraManager::GetDrawingAreaSize_CameraIndex(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			const std::string text = "No Contest";
			const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
			DrawExtendFormatString(screenCenter.x - width * 0.5f, screenCenter.y, rectTransform->scale.x, rectTransform->scale.y, GetColor(255, 255, 255), text.c_str());
		}
	}
}

void UI_Ranking::ChangeState(STATE state) {

	m_PrevState = m_CurrentState;
	m_CurrentState = state;

	switch (m_CurrentState)
	{
	case UI_Ranking::STATE::S_FADE_IN:

		//for (const auto& score : m_UI_Scores) {
		//	score->SetEasing(UI_Canvas::eMove, Vector2(0.0f, 0.0f), Vector2(WindowSetting::Inst().width_half, WindowSetting::Inst().height_half + 150.0f), 1.0f);
		//}

		break;

	case UI_Ranking::STATE::S_DISPLAY:
		break;

	default:
		break;
	}
}
