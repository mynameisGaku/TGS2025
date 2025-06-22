#include "src/reference/ball/BallRef.h"
#include "src/util/file/json/settings_json.h"

namespace
{
	const char* PATH = "data/Json/Ball/Ball.json";
	const char* FILEKEY = "Ball";
}

void BallRef::Load()
{
	//=== Json“Ç‚Ýž‚Ý ===
	Settings_json* settingsJson = Settings_json::Inst();
	settingsJson->LoadSettingJson(PATH, FILEKEY);

	//=== ƒpƒ‰ƒ[ƒ^ ===
	Gravity					= settingsJson->GetOrDefault("Gravity", Vector3(0, 0, 0), FILEKEY);
	Friction				= settingsJson->GetOrDefault("Friction", Vector3(0, 0, 0), FILEKEY);
	Bounciness				= settingsJson->GetOrDefault("Bounciness", 0.0f, FILEKEY);
	SpeedDefault			= settingsJson->GetOrDefault("SpeedDefault", 0.0f, FILEKEY);

	DropForce_Horizontal	= settingsJson->GetOrDefault("DropForce_Horizontal", 0.0f, FILEKEY);
	DropForce_Vertical		= settingsJson->GetOrDefault("DropForce_Vertical", 0.0f, FILEKEY);

	LifeTimeMax				= settingsJson->GetOrDefault("LifeTimeMax", 0.0f, FILEKEY);
	PoolMax					= settingsJson->GetOrDefault("PoolMax", 0, FILEKEY);
}
