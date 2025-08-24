#include "UI_MatchTimeRef.h"
#include <src/util/file/json/settings_json.h>
#include <src/util/string/StringUtil.h>

void UI_MatchTimeRef::Load(bool forceLoad)
{
	if (!forceLoad && m_WasLoad) return;

	// ロード処理
	auto jsonLoader = Settings_json::Inst();
	jsonLoader->LoadSettingJson(PATH, FILEKEY);
	auto& jsonData = jsonLoader->GetJson(FILEKEY);

	// 各種データを取り出す処理
	PositionY = jsonLoader->GetOrDefault("Param.PositionY", 0.0f, FILEKEY);
	BackScale = jsonLoader->GetOrDefault("Param.BackScale", Vector2::Zero, FILEKEY);
	BackColor = jsonLoader->GetOrDefault("Param.BackColor", RGBColor(), FILEKEY);
	TimeNormalColor = jsonLoader->GetOrDefault("Param.TimeNormalColor", RGBColor(), FILEKEY);
	TimeWarningColor = jsonLoader->GetOrDefault("Param.TimeWarningColor", RGBColor(), FILEKEY);

	m_WasLoad = true;
}
