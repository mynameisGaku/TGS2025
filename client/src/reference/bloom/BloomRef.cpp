#include <vendor/nlohmann/json.hpp>
#include "src/util/file/json/settings_json.h"
#include "src/reference/bloom/BloomRef.h"

BloomRef* BloomRef::instance = nullptr;

namespace
{
}

void BloomRef::Load()
{
	//=== Json“Ç‚Ýž‚Ý ===
	Settings_json* settingsJson = Settings_json::Inst();
	settingsJson->LoadSettingJson(PATH, FILEKEY);

	//=== ƒpƒ‰ƒ[ƒ^ ===
	Param = settingsJson->GetOrDefault("Param", BloomRef::Parameter(), FILEKEY);
}

void to_json(nlohmann::json& j, const BloomRef::Parameter& param)
{
	j = nlohmann::json{
		{ "DownScale", param.DownScale },
		{ "MinBrightness", param.MinBrightness },
		{ "GaussianRate", param.GaussianRate },
		{ "AddRate", param.AddRate },
	};
}

void from_json(const nlohmann::json& j, BloomRef::Parameter& param)
{
	j.at("DownScale").get_to(param.DownScale);
	j.at("MinBrightness").get_to(param.MinBrightness);
	j.at("GaussianRate").get_to(param.GaussianRate);
	j.at("AddRate").get_to(param.AddRate);
}
