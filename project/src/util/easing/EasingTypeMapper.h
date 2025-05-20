#pragma once
#include <string>
#include <unordered_map>
#include "src/util/easing/EasingType.h"

inline EasingType EasingTypeFromString(const std::string& str) {
    static const std::unordered_map<std::string, EasingType> map = {
        {"Linear", EasingType::Linear},
        {"EaseInSine", EasingType::InSine},
        {"EaseOutSine", EasingType::OutSine},
        {"EaseInOutSine", EasingType::InOutSine},
        {"EaseInQuad", EasingType::InQuad},
        {"EaseOutQuad", EasingType::OutQuad},
        {"EaseInOutQuad", EasingType::InOutQuad},
        {"EaseInCubic", EasingType::InCubic},
        {"EaseOutCubic", EasingType::OutCubic},
        {"EaseInOutCubic", EasingType::InOutCubic},
        {"EaseInQuart", EasingType::InQuart},
        {"EaseOutQuart", EasingType::OutQuart},
        {"EaseInOutQuart", EasingType::InOutQuart},
        {"EaseInQuint", EasingType::InQuint},
        {"EaseOutQuint", EasingType::OutQuint},
        {"EaseInOutQuint", EasingType::InOutQuint},
        {"EaseInExpo", EasingType::InExpo},
        {"EaseOutExpo", EasingType::OutExpo},
        {"EaseInOutExpo", EasingType::InOutExpo},
        {"EaseInCirc", EasingType::InCirc},
        {"EaseOutCirc", EasingType::OutCirc},
        {"EaseInOutCirc", EasingType::InOutCirc},
        {"EaseInBack", EasingType::InBack},
        {"EaseOutBack", EasingType::OutBack},
        {"EaseInOutBack", EasingType::InOutBack},
        {"EaseInElastic", EasingType::InElastic},
        {"EaseOutElastic", EasingType::OutElastic},
        {"EaseInOutElastic", EasingType::InOutElastic},
        {"EaseInBounce", EasingType::InBounce},
        {"EaseOutBounce", EasingType::OutBounce},
        {"EaseInOutBounce", EasingType::InOutBounce}
    };

    auto it = map.find(str);
    if (it != map.end()) return it->second;
    return EasingType::Linear; // fallback
}

inline const std::string& EasingTypeToString(EasingType type) {
    static const std::unordered_map<EasingType, std::string> reverseMap = {
        {EasingType::Linear, "Linear"},
        {EasingType::InSine, "EaseInSine"},
        {EasingType::OutSine, "EaseOutSine"},
        {EasingType::InOutSine, "EaseInOutSine"},
        {EasingType::InQuad, "EaseInQuad"},
        {EasingType::OutQuad, "EaseOutQuad"},
        {EasingType::InOutQuad, "EaseInOutQuad"},
        {EasingType::InCubic, "EaseInCubic"},
        {EasingType::OutCubic, "EaseOutCubic"},
        {EasingType::InOutCubic, "EaseInOutCubic"},
        {EasingType::InQuart, "EaseInQuart"},
        {EasingType::OutQuart, "EaseOutQuart"},
        {EasingType::InOutQuart, "EaseInOutQuart"},
        {EasingType::InQuint, "EaseInQuint"},
        {EasingType::OutQuint, "EaseOutQuint"},
        {EasingType::InOutQuint, "EaseInOutQuint"},
        {EasingType::InExpo, "EaseInExpo"},
        {EasingType::OutExpo, "EaseOutExpo"},
        {EasingType::InOutExpo, "EaseInOutExpo"},
        {EasingType::InCirc, "EaseInCirc"},
        {EasingType::OutCirc, "EaseOutCirc"},
        {EasingType::InOutCirc, "EaseInOutCirc"},
        {EasingType::InBack, "EaseInBack"},
        {EasingType::OutBack, "EaseOutBack"},
        {EasingType::InOutBack, "EaseInOutBack"},
        {EasingType::InElastic, "EaseInElastic"},
        {EasingType::OutElastic, "EaseOutElastic"},
        {EasingType::InOutElastic, "EaseInOutElastic"},
        {EasingType::InBounce, "EaseInBounce"},
        {EasingType::OutBounce, "EaseOutBounce"},
        {EasingType::InOutBounce, "EaseInOutBounce"}
    };

    auto it = reverseMap.find(type);
    if (it != reverseMap.end()) return it->second;
    static const std::string unknown = "Unknown";
    return unknown;
}
