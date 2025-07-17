#pragma once
#include <vendor/nlohmann/json_fwd.hpp>

class Vector2;
class Vector3;

void to_json(nlohmann::json& j, const Vector2& vec);
void to_json(nlohmann::json& j, const Vector3& vec);
void from_json(const nlohmann::json& j, Vector2& vec);
void from_json(const nlohmann::json& j, Vector3& vec);
