#include "UI_ChatBarRef.h"
#include <src/util/file/json/settings_json.h>
#include <src/util/string/StringUtil.h>

void UI_ChatBarRef::Load(bool forceLoad)
{
	if (!forceLoad && m_WasLoad) return;

	// ロード処理
	auto jsonLoader = Settings_json::Inst();
	jsonLoader->LoadSettingJson(PATH, FILEKEY);
	auto& jsonData = jsonLoader->GetJson(FILEKEY);

	// 各種データを取り出す処理
	PositionY = jsonLoader->GetOrDefault("Param.PositionY", 0.0f, FILEKEY);
	IconOffsetX = jsonLoader->GetOrDefault("Param.IconOffsetX", 0.0f, FILEKEY);
	IconScale = jsonLoader->GetOrDefault("Param.IconScale", 0.0f, FILEKEY);
	BackScale = jsonLoader->GetOrDefault("Param.BackScale", Vector2::Zero, FILEKEY);
	BackColor = jsonLoader->GetOrDefault("Param.BackColor", RGBColor(), FILEKEY);
	GaugeBackScale = jsonLoader->GetOrDefault("Param.GaugeBackScale", Vector2::Zero, FILEKEY);
	GaugeBackColor = jsonLoader->GetOrDefault("Param.GaugeBackColor", RGBColor(), FILEKEY);

	m_WasLoad = true;
}
