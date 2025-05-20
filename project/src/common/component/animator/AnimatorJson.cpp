#include <vendor/nlohmann/json.hpp>
#include "src/common/component/animator/AnimatorJson.h"
#include "src/common/component/animator/AnimationDefine.h"

#if FALSE
void to_json(nlohmann::json& j, const AnimOption& opt)
{
	j = nlohmann::json{
		{ "X", vec.x },
		{ "Y", vec.y }
	};
}

void from_json(const nlohmann::json& j, AnimOption& opt)
{
	j.at("DefaultAnimSpeed").<float, >(opt.defaultAnimSpeed);
	j.at("IsFixedRoot").at("X").get_to<bool, false>(opt.isFixedRoot[0]);
	j.at("IsFixedRoot").at("Y").get_to<bool, false>(opt.isFixedRoot[1]);
	j.at("IsFixedRoot").at("Z").get_to<bool, false>(opt.isFixedRoot[2]);
}
#endif // FALSE
