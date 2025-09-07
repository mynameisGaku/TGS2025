#include "UI_Setter_ResultScene.h"
#include "src/scene/result/ui/UI_Winner_Team.h"
#include "src/scene/result/ui/UI_Ranking.h"

#include "src/common/camera/CameraManager.h"

UI_Setter_ResultScene::UI_Setter_ResultScene()
{
	UI_Winner_Team* winner = new UI_Winner_Team();
	winner->rectTransform->scale = Vector2::Ones * 2.0f;
	
	UI_Ranking* ranking = new UI_Ranking();
	ranking->rectTransform->anchor.SetPreset(Anchor::Preset::Middle);
	ranking->rectTransform->position = Vector2(0.0f, 150.0f);
	ranking->ChangeState(UI_Ranking::STATE::S_NONE);

	m_UIs["UI_Winner_Team"] = winner;
	m_UIs["UI_Ranking"] = ranking;
}

UI_Setter_ResultScene::~UI_Setter_ResultScene()
{
	for (auto ui = m_UIs.begin(); ui != m_UIs.end();)
	{
		delete ui->second;
		ui = m_UIs.erase(ui);
	}
}

void UI_Setter_ResultScene::Update()
{
	if (not CameraManager::IsPlayingPerformance()) {
		UI_Ranking* ranking = dynamic_cast<UI_Ranking*>(m_UIs["UI_Ranking"]);
		if (ranking != nullptr &&
			ranking->CurrentState() != UI_Ranking::STATE::S_FADE_IN &&
			ranking->PrevState() != UI_Ranking::STATE::S_FADE_IN)
			ranking->ChangeState(UI_Ranking::STATE::S_FADE_IN);
	}
}

void UI_Setter_ResultScene::Draw()
{
	
}