#include "UI_Setter_ResultScene.h"
#include "src/scene/result/ui/UI_Winner_Team.h"
#include "src/scene/result/ui/UI_Ranking.h"

UI_Setter_ResultScene::UI_Setter_ResultScene()
{
	UI_Winner_Team* winner = new UI_Winner_Team();
	UI_Ranking* ranking = new UI_Ranking();

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
}

void UI_Setter_ResultScene::Draw()
{
	
}