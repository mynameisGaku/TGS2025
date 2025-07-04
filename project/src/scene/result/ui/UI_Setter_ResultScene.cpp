#include "UI_Setter_ResultScene.h"

#include "src/util/ui/UI_Manager.h"

UI_Setter_ResultScene::UI_Setter_ResultScene()
{
	m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();

	// Ÿ—˜‰‰o‰æ‘œ‚ðÝ’è
	m_UI_Result_Won = new UI_Canvas();
	m_UI_Result_Won->rectTransform->anchor.SetPreset(Anchor::Preset::Middle);
	m_UI_Result_Won->SetTag("UI_Result_Won");
	m_UI_Result_Won->SetImage(LoadGraph("data/texture/UI/Result/YouWon.png"));
	m_UI_Result_Won->SetEasing(UI_Canvas::eAlpha, 0, 255, 1.0f, EasingType::Linear);
	m_UI_Result_Won->SetEasing(UI_Canvas::eScale, Vector2::Zero, Vector2::Ones, 1.0f, EasingType::Linear);
}

UI_Setter_ResultScene::~UI_Setter_ResultScene()
{
	if (m_UI_Result_Won != nullptr)
		delete m_UI_Result_Won;
	m_UI_Result_Won = nullptr;
}

void UI_Setter_ResultScene::Update()
{
}

void UI_Setter_ResultScene::Draw()
{
	if (not m_pGameManager->GetResultData().WinnerTeamName.empty())
		DrawFormatStringF(100, 100, 0xFF0000, (m_pGameManager->GetResultData().WinnerTeamName[0] + " Team Won!").c_str());
	else
		DrawFormatStringF(100, 100, 0xFF0000, "No Contest");
}