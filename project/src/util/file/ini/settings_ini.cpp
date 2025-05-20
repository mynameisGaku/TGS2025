// settings_ini.cpp
#include "src/util/file/ini/settings_ini.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <vendor/imgui/ImGui.h>

Settings_ini::Settings_ini()
{
}

Settings_ini::~Settings_ini()
{
    for (auto& f : iniFiles_)
    {
        f.second.clear();
    }
    iniFiles_.clear();  ///< ファイルごとのキーと値のマップ
    modificationTimes_.clear();      ///< ファイルの最終変更時間の記録
    listeners_.clear();                                                  ///< 設定変更リスナーのリスト
    activeFile_.clear();                                                                  ///< 現在アクティブなファイルのキー
    
    StopWatcher();
}

Settings_ini& Settings_ini::Inst()
{
    static Settings_ini instance;
    return instance;
}

void Settings_ini::Load(const std::string& filepath, const std::string& key, bool setActive)
{
    std::ifstream file(filepath);

    if (!file)
    {
        std::cerr << "[エラー] INI ファイルを開けません: " << filepath << std::endl;
        return;
    }

    std::unordered_map<std::string, std::string> values;
    std::string line, section;

    while (std::getline(file, line))
    {
        line = Trim(line);

        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            continue;
        }

        if (line.front() == '[' && line.back() == ']')
        {
            section = line.substr(1, line.size() - 2);
            continue;
        }

        auto delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos)
        {
            std::string keyName = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            std::string fullKey = section.empty() ? keyName : section + "." + keyName;
            values[fullKey] = value;
        }
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        iniFiles_[key] = std::move(values);
        modificationTimes_[filepath] = std::filesystem::last_write_time(filepath);
    }

    if (setActive || activeFile_.empty())
    {
        activeFile_ = key;
    }

    NotifyChange(key);
}

void Settings_ini::Save(const std::string& filepath, const std::string& fileKey)
{
    const auto& values = GetFileValues(fileKey);
    std::ofstream ofs(filepath);

    if (!ofs)
    {
        std::cerr << "[エラー] ファイルに書き込みできません: " << filepath << std::endl;
        return;
    }

    std::string currentSection;
    for (const auto& [fullKey, value] : values)
    {
        auto pos = fullKey.find('.');
        std::string section = (pos != std::string::npos) ? fullKey.substr(0, pos) : "";

        if (section != currentSection)
        {
            if (!currentSection.empty())
            {
                ofs << "\n";
            }

            ofs << "[" << section << "]\n";
            currentSection = section;
        }

        std::string keyName = (pos != std::string::npos) ? fullKey.substr(pos + 1) : fullKey;
        ofs << keyName << "=" << value << "\n";
    }
}

void Settings_ini::AddChangeListener(ChangeCallback callback)
{
    listeners_.push_back(std::move(callback));
}

void Settings_ini::NotifyChange(const std::string& key)
{
    for (auto& cb : listeners_)
    {
        cb(key);
    }
}

void Settings_ini::StartWatcher()
{
    if (watcherRunning_)
    {
        return;
    }

    watcherRunning_ = true;

    std::thread([this]()
        {
            WatchLoop();
        }).detach();
}

void Settings_ini::StopWatcher()
{
    watcherRunning_ = false;
}

void Settings_ini::WatchLoop()
{
    while (watcherRunning_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto& [fileKey, _] : iniFiles_)
        {
            std::string filepath = fileKey + ".ini";
            auto currentTime = std::filesystem::last_write_time(filepath);

            if (modificationTimes_[filepath] != currentTime)
            {
                Load(filepath, fileKey);
            }
        }
    }
}

const std::unordered_map<std::string, std::string>& Settings_ini::GetFileValues(const std::string& key) const
{
    const std::string& targetKey = key.empty() ? activeFile_ : key;

    auto it = iniFiles_.find(targetKey);
    if (it == iniFiles_.end())
    {
        throw std::runtime_error("[エラー] 対象ファイルが読み込まれていません: " + targetKey);
    }

    return it->second;
}

std::string Settings_ini::Trim(const std::string& s)
{
    const char* whitespace = " \t\n\r";
    size_t start = s.find_first_not_of(whitespace);
    size_t end = s.find_last_not_of(whitespace);

    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

void Settings_ini::RenderImGui(const std::string& fileKey)
{
    auto& values = iniFiles_[fileKey.empty() ? activeFile_ : fileKey];

    if (ImGui::Begin("INI 設定エディター"))
    {
        for (auto& [key, value] : values)
        {
            float val = std::stof(value);

            if (ImGui::DragFloat(key.c_str(), &val, 0.1f))
            {
                value = std::to_string(val);
                NotifyChange(key);
            }
        }

        if (ImGui::Button("保存"))
        {
            Save(fileKey + ".ini", fileKey);
        }
    }

    ImGui::End();
}
