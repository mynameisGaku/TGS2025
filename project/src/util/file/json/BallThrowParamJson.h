#pragma once
#include <vendor/nlohmann/json_fwd.hpp>

class BallThrowParam;

void to_json(nlohmann::json& j, const BallThrowParam& param);
void from_json(const nlohmann::json& j, BallThrowParam& param);
