#include "UI_Winner_Team.h"

#include "src/util/screen/ScreenManager.h"

#include "src/common/game/GameManager.h"
#include "src/common/camera/CameraManager.h"

UI_Winner_Team::UI_Winner_Team()
{
	m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();

	if (m_pGameManager != nullptr)
	{
		const GameManager::ResultData resultData = m_pGameManager->GetResultData();
		// リザルトデータに勝者がいない場合
		if (resultData.WinnerTeamName.empty())
		{
			const std::string path = "data/texture/UI/Result/DrawGame.png";
			hResult = LoadGraph(path.c_str());
		}
		else
		{
			const std::string path = "data/texture/UI/Result/TeamWin_" + resultData.WinnerTeamName[0] + ".png";
			hResult = LoadGraph(path.c_str());
		}
	}
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

	for (int i = 0; i < CAMERA_NUM; ++i)
	{
		Vector2 screenBegin = CameraManager::GetDrawingAreaPos_CameraIndex(i);
		Vector2 screenEnd = screenBegin + CameraManager::GetDrawingAreaSize_CameraIndex(i);
		Vector2 screenCenter = screenBegin + (screenEnd - screenBegin) * 0.5f;
		const Vector2 base = Vector2(screenCenter.x, screenCenter.y - 150.0f);

		int w, h;
		GetGraphSize(hResult, &w, &h);
		DrawRectRotaGraphF(base.x, base.y, 0, 0, w, h, 1.0f, 0.0f, hResult, TRUE);
	}
}