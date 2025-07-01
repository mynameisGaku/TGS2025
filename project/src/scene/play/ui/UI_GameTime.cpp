#include "UI_GameTime.h"
#include "src/reference/game/GameRef.h"
#include "src/common/game/GameManager.h"
#include "src/util/string/StringUtil.h"
#include "src/util/ui/UI_Manager.h"

UI_GameTime::UI_GameTime() : UI_GameTime(RectTransform(), 0)
{
}

UI_GameTime::UI_GameTime(const RectTransform& trs, int index)
{
	m_pGameManager = nullptr;
	m_CharaIndex = index;

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);
}

UI_GameTime::~UI_GameTime()
{
}

void UI_GameTime::Update()
{

}

void UI_GameTime::Draw()
{
	if (m_pGameManager == nullptr) {
		m_pGameManager = SceneManager::CommonScene()->FindGameObject<GameManager>();

		if (m_pGameManager == nullptr)
			return;
	}

	std::string text = StringUtil::FormatToString("Žc‚èŽžŠÔ:%0.2f", m_pGameManager->GetCurrentGameTime()).c_str();
	int width = GetDrawStringWidth(text.c_str(), text.length());

	const Vector2 adjust = Vector2(0, 0);
	const RectTransform globalTrs = rectTransform->Global();

	DrawBoxAA(
		globalTrs.position.x - width,
		globalTrs.position.y - adjust.y * 0.5f,
		globalTrs.position.x,
		globalTrs.position.y + adjust.y * 0.5f,
		GetColor(120, 120, 120), true);

	DrawFormatString(
		globalTrs.position.x - width,
		globalTrs.position.y - adjust.y * 0.5f,
		GetColor(255, 255, 255),
		text.c_str());
}