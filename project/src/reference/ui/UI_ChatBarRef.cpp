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

	m_WasLoad = true;
}
