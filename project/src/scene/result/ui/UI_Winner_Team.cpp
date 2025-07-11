#include "UI_Winner_Team.h"

#include "src/util/screen/ScreenManager.h"

#include "src/common/game/GameManager.h"
#include "src/common/camera/CameraManager.h"

UI_Winner_Team::UI_Winner_Team()
{
	m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();
}

UI_Winner_Team::~UI_Winner_Team()
{
}

void UI_Winner_Team::Update()
{

}

void UI_Winner_Team::Draw()
{
	const GameManager::ResultData resultData = m_pGameManager->GetResultData();
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	// リザルトデータに勝者がいる場合
	if (not resultData.WinnerTeamName.empty())
	{
		for (int i = 0; i < CAMERA_NUM; ++i)
		{
			Vector2 screenBegin = ScreenManager::GetScreenBeginPos(i);
			Vector2 screenEnd = ScreenManager::GetScreenEndPos(i);
			Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;

			const std::string text = (resultData.WinnerTeamName[0] + " Team Won!");
			const int width = (int)(GetDrawStringWidth(text.c_str(), text.length()) * rectTransform->scale.Average());
			const std::string teamName = resultData.CharaInTeamName.at(resultData.WinnerCharaIDs[0]);
			const int color = resultData.TeamColor.at(teamName);

			const Vector2 base = Vector2(screenCenter.x - width * 0.5f, screenCenter.y - 200.0f);

			DrawExtendFormatString(base.x, base.y, rectTransform->scale.x, rectTransform->scale.y, color, text.c_str());
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