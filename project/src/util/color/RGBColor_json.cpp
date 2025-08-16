#include "RGBColor_json.h"
#include "vendor/nlohmann/json.hpp"
#include "RGBColor.h"

void to_json(nlohmann::json& j, const RGBColor& rgbColor)
{
	j = nlohmann::json{
		{ "R", rgbColor.GetRed() },
		{ "G", rgbColor.GetGreen() },
		{ "B", rgbColor.GetBlue() },
	};
}

void from_json(const nlohmann::json& j, RGBColor& rgbColor)
{
	int r, g, b;
	j.at("R").get_to(r);
	j.at("G").get_to(g);
	j.at("B").get_to(b);

	rgbColor.SetColorRGB(r, g, b);
}
