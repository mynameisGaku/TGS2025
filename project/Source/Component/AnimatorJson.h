#pragma once
#include "nlohmann/json_fwd.hpp"

#if FALSE
struct AnimOption;

void to_json(nlohmann::json& j, const AnimOption& opt);
void from_json(const nlohmann::json& j, AnimOption& opt);
#endif // FALSE