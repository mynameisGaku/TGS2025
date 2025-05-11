#include "BallRef.h"
#include "settings_json.h"

namespace
{
}

void BallRef::Load()
{
	//=== Json“Ç‚Ýž‚Ý ===
	Settings_json* settingsJson = Settings_json::Inst();
	settingsJson->LoadSettingJson(PATH, FILEKEY);

	//=== ƒpƒ‰ƒ[ƒ^ ===
	GravityDefault		= settingsJson->GetOrDefault("GravityDefault", Vector3(0, 0, 0), FILEKEY);
	FrictionDefault		= settingsJson->GetOrDefault("FrictionDefault", Vector3(0, 0, 0), FILEKEY);
	BouncinessDefault	= settingsJson->GetOrDefault("BouncinessDefault", 0.0f, FILEKEY);
	SpeedDefault		= settingsJson->GetOrDefault("SpeedDefault", 0.0f, FILEKEY);
	HomingTimeMax		= settingsJson->GetOrDefault("HomingTimeMax", 0.0f, FILEKEY);
	LifeTimeMax			= settingsJson->GetOrDefault("LifeTimeMax", 0.0f, FILEKEY);
	Max					= settingsJson->GetOrDefault("Max", 0, FILEKEY);
}
