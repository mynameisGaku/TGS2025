#include "MatchRef.h"
#include "src/util/file/json/settings_json.h"

namespace
{
	const char* PATH = "data/Json/Match/Match.json";
	const char* FILEKEY = "Match";
}

void MatchRef::Load()
{
	//=== Json�ǂݍ��� ===
	Settings_json* settingsJson = Settings_json::Inst();
	settingsJson->LoadSettingJson(PATH, FILEKEY);

	//=== �p�����[�^ ===
	GameOver_Duration = settingsJson->GetOrDefault("GameOver_Duration", 0.0f, FILEKEY);
}
