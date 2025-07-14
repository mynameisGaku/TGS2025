#include "BallThrowParamJson.h"
#include <vendor/nlohmann/json.hpp>
#include "src/reference/ball/BallRef.h"

void to_json(nlohmann::json& j, const BallThrowParam& param)
{
	j = nlohmann::json{
		{ "ChargeLevel", param.ChargeLevel },
		{ "Speed", param.Speed },
		{ "EffectScale", param.EffectScale },
		{ "EffectSpeed", param.EffectSpeed },
	};
}

void from_json(const nlohmann::json& j, BallThrowParam& param)
{
	j.at("ChargeLevel").get_to(param.ChargeLevel);
	j.at("Speed").get_to(param.Speed);
	j.at("EffectScale").get_to(param.EffectScale);
	j.at("EffectSpeed").get_to(param.EffectSpeed);
}
