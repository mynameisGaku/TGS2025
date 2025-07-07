#pragma once
#include <vector>
#include <any>
#include <string>
#include <vendor/nlohmann/json.hpp>

using json = nlohmann::json;

class RawJson
{
public:
    // �f�[�^����JSON���\�z���ĕԂ�
    static json Create(const std::vector<std::any>& values);

    // �L�[�t��JSON�I�u�W�F�N�g���\�z���ĕԂ�
    static json CreateObject(const std::vector<std::pair<std::string, std::any>>& keyValues);

    // JSON�����񂩂ǂ����`�F�b�N����
    static bool CheckIsJson(const std::string& str);
};