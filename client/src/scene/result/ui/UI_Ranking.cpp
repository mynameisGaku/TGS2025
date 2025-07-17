#include "UI_Ranking.h"

#include "src/util/screen/ScreenManager.h"

#include "src/common/game/GameManager.h"
#include "src/common/camera/CameraManager.h"

UI_Ranking::UI_Ranking()
{
	m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();
}

UI_Ranking::~UI_Ranking()
{
}

void UI_Ranking::Update()
{

}

void UI_Ranking::Draw()
{
	const GameManager::ResultData resultData = m_pGameManager->GetResultData();
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	// リザルトデータに勝者がいる場合
	if (not resultData.Ranking.empty())
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = ScreenManager::GetScreenBeginPos(i);
			Vector2 screenEnd = ScreenManager::GetScreenEndPos(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			// 最大表示数
			const int DISP_MAX = 5;

			for (int j = 0; j < DISP_MAX; j++)
			{
				if (j >= resultData.Ranking.size()) break;

				int charaId = resultData.Ranking[j].first;
				int score = resultData.Ranking[j].second;

				const std::string text = ("No." + std::to_string(charaId) + " :Score." + std::to_string(score));
				const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
                const std::string teamName = resultData.CharaInTeamName.at(charaId);
				const int color = resultData.TeamColor.at(teamName);

				const Vector2 base = Vector2(screenCenter.x - width * 0.5f, screenCenter.y + 150.0f);
				const Vector2 offset = Vector2(0.0f, 30.0f * j);

				DrawExtendFormatString(base.x + offset.x, base.y + offset.y, rectTransform->scale.x, rectTransform->scale.y, color, text.c_str());
			}
		}
	}
	else
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = ScreenManager::GetScreenBeginPos(i);
			Vector2 screenEnd = ScreenManager::GetScreenEndPos(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			const std::string text = "No Contest";
			const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
			DrawExtendFormatString(screenCenter.x - width * 0.5f, screenCenter.y, rectTransform->scale.x, rectTransform->scale.y, GetColor(255, 255, 255), text.c_str());
		}
	}
}