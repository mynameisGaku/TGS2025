#include "src/reference/ball/BallRef.h"
#include "src/util/file/json/settings_json.h"

namespace
{
	const char* PATH = "data/Json/Ball/Ball.json";
	const char* FILEKEY = "Ball";
}

void BallRef::Load()
{
	//=== Json読み込み ===
	Settings_json* settingsJson = Settings_json::Inst();
	settingsJson->LoadSettingJson(PATH, FILEKEY);

	//=== パラメータ ===
	Gravity					= settingsJson->GetOrDefault("Gravity", Vector3(0, 0, 0), FILEKEY);
	Friction				= settingsJson->GetOrDefault("Friction", Vector3(0, 0, 0), FILEKEY);
	Bounciness				= settingsJson->GetOrDefault("Bounciness", 0.0f, FILEKEY);
	ChargeLevels			= settingsJson->GetOrDefault("ChargeLevels", std::vector<BallThrowParam>(), FILEKEY);

	DropForce_Horizontal	= settingsJson->GetOrDefault("DropForce_Horizontal", 0.0f, FILEKEY);
	DropForce_Vertical		= settingsJson->GetOrDefault("DropForce_Vertical", 0.0f, FILEKEY);

	LifeTimeMax				= settingsJson->GetOrDefault("LifeTimeMax", 0.0f, FILEKEY);
	PoolMax					= settingsJson->GetOrDefault("PoolMax", 0, FILEKEY);
}
