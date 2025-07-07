#pragma once
#include <vector>
#include <any>
#include <string>
#include <vendor/nlohmann/json.hpp>

using json = nlohmann::json;

class RawJson
{
public:
    // データからJSONを構築して返す
    static json Create(const std::vector<std::any>& values);

    // キー付きJSONオブジェクトを構築して返す
    static json CreateObject(const std::vector<std::pair<std::string, std::any>>& keyValues);

    // JSON文字列かどうかチェックする
    static bool CheckIsJson(const std::string& str);
};