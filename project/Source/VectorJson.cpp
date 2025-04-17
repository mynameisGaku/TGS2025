#include "VectorJson.h"
#include "Util/Vector2.h"
#include "Util/Vector3.h"
#include "nlohmann/json.hpp"

void to_json(nlohmann::json& j, const Vector2& vec)
{
	j = nlohmann::json{
		{ "X", vec.x },
		{ "Y", vec.y }
	};
}

void to_json(nlohmann::json& j, const Vector3& vec)
{
	j = nlohmann::json{
		{ "X", vec.x },
		{ "Y", vec.y },
		{ "Z", vec.z }
	};
}

void from_json(const nlohmann::json& j, Vector2& vec)
{
	j.at("X").get_to(vec.x);
	j.at("Y").get_to(vec.y);
}

void from_json(const nlohmann::json& j, Vector3& vec)
{
	j.at("X").get_to(vec.x);
	j.at("Y").get_to(vec.y);
	j.at("Z").get_to(vec.z);
}
