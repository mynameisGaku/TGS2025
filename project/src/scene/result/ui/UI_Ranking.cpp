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
	if (not resultData.Chara_TopScore.empty())
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = ScreenManager::GetScreenBeginPos(i);
			Vector2 screenEnd = ScreenManager::GetScreenEndPos(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			// 表示数
			for (int j = 0; j < 5; j++)
			{
				if (j >= resultData.Chara_TopScore.size()) break;

				int charaId = resultData.Chara_TopScore[j].first;
				int score = resultData.Chara_TopScore[j].second;

				const std::string text = ("No." + std::to_string(charaId) + " :Score." + std::to_string(score));
				const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
				DrawExtendFormatString(screenCenter.x - width * 0.5f, screenCenter.y + 100.0f + 30.0f * j, rectTransform->scale.x, rectTransform->scale.y, resultData.TeamColor[charaId], text.c_str());
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