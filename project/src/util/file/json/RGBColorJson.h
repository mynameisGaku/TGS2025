#pragma once
#include "vendor/nlohmann/json_fwd.hpp"

class RGBColor;

void to_json(nlohmann::json& j, const RGBColor& rgbColor);
void from_json(const nlohmann::json& j, RGBColor& rgbColor);
