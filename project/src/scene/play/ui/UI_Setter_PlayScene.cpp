#include "UI_Setter_PlayScene.h"

#include "src/common/camera/CameraManager.h"

#include "src/scene/play/ui/UI_CrossHair.h"
#include "src/scene/play/ui/UI_HitPoint_Icon.h"
#include "src/scene/play/ui/UI_GameScore.h"
#include "src/scene/play/ui/UI_MatchTime.h"
#include "src/scene/play/ui/UI_ButtonHint.h"
#include "src/scene/play/ui/UI_StartCount.h"
#include "src/scene/play/ui/UI_Fade.h"

UI_Setter_PlayScene::UI_Setter_PlayScene()
{
	const int CAMERA_NUM = (int)CameraManager::AllCameras().size();

	for (int i = 0; i < CAMERA_NUM; i++)
	{
		const std::string sPlayerNum = std::to_string(i + 1) + "P";

		UI_CrossHair* crossHair = new UI_CrossHair(RectTransform(Anchor::Preset::Middle), i);
		crossHair->SetScroll(nullptr, 0.0f, 1.0f, Gauge::ScrollType::eUp, false);
		crossHair->SetTag("CrossHair_" + sPlayerNum);

		UI_HitPoint_Icon* hitPointIcon = new UI_HitPoint_Icon(RectTransform(Anchor::Preset::LeftDown, Vector2::Zero, 0.0f, Vector2::Ones * 2.0f), i);
		hitPointIcon->SetValue(nullptr, 0.0f, 0.0f, 1.0f);
		hitPointIcon->SetTag("HitPoint_Icon_" + sPlayerNum);

		UI_MatchTime* matchTime = new UI_MatchTime(RectTransform(Anchor::Preset::RightDown, Vector2(0.0f, -165.0f)), i);
		matchTime->SetTag("MatchTime_" + sPlayerNum);

		UI_ButtonHint* buttonHint = new UI_ButtonHint(RectTransform(Anchor::Preset::Middle, Vector2(0.0f, -100.0f)), i);
		buttonHint->SetTag("ButtonHint_" + sPlayerNum);

		UI_StartCount* startCount = new UI_StartCount(RectTransform(Anchor::Preset::Middle, Vector2(0.0f, -100.0f)), i);
		startCount->SetTag("StartCount_" + sPlayerNum);

		UI_FadeBase* fade = new UI_FadeBlack(RectTransform(Anchor::Preset::Middle, Vector2(0.0f, 0.0f)), i);
		fade->SetTag("Fade_" + sPlayerNum);

		m_UIs[crossHair->GetTag()]		= crossHair;
		m_UIs[hitPointIcon->GetTag()]	= hitPointIcon;
		m_UIs[matchTime->GetTag()]		= matchTime;
		m_UIs[buttonHint->GetTag()]		= buttonHint;
		m_UIs[startCount->GetTag()]		= startCount;
		m_UIs[fade->GetTag()]			= fade;
	}
}

UI_Setter_PlayScene::~UI_Setter_PlayScene()
{
	for (auto ui = m_UIs.begin(); ui != m_UIs.end();)
	{
		delete ui->second;
		ui = m_UIs.erase(ui);
	}
}
