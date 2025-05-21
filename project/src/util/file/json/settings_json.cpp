#include "src/util/file/json/settings_json.h"

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

/// <summary>
/// wstringからstringへの変換 (UTF-8)
/// </summary>
std::string WStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size_needed - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, result.data(), size_needed, nullptr, nullptr);
    return result;
}

Settings_json* Settings_json::m_Inst = nullptr;

Settings_json::~Settings_json()
{
    for (auto& json : m_JsonFiles)
    {
        json.second.jsonContent.clear();
    }
    m_JsonFiles.clear();
    m_FileModificationTimes.clear(); // ファイル変更時間の管理用マップ。
    m_ChangeListeners.clear();
}

Settings_json* Settings_json::Inst()
{
    if (!m_Inst)
    {
        m_Inst = new Settings_json();
    }
    return m_Inst;
}

void Settings_json::Destroy()
{
    if (m_Inst)
    {
        m_Inst->StopFileWatcher();
        delete m_Inst;
        m_Inst = nullptr;
    }
}

void Settings_json::SetOutputDirectory(const std::string& directory)
{
    if (!std::filesystem::exists(directory))
    {
        std::filesystem::create_directories(directory);
    }
    m_OutputDirectory = directory;
}

void Settings_json::LoadSettingJson(const std::string& filepath, const std::string& fileKey, bool setActive)
{
    std::ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        std::cerr << "[Error] ファイルを開けませんでした: " << filepath << std::endl;
        return;
    }

    nlohmann::json jsonData;
    try
    {
        ifs >> jsonData;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Error] JSONパースに失敗しました: " << e.what() << std::endl;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_JsonFiles[fileKey] = { std::move(jsonData), filepath };
        m_FileModificationTimes[fileKey] = std::filesystem::last_write_time(filepath);
    }

    if (setActive)
    {
        m_ActiveFileKey = fileKey;
    }

    NotifyChange(fileKey);
}

bool Settings_json::IsFileInUse(const std::string& filepath) const
{
#ifdef _WIN32
    HANDLE fileHandle = CreateFileA(
        filepath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        return true; // 使用中
    }

    CloseHandle(fileHandle);
#endif
    return false; // 使用されていない
}

const nlohmann::json& Settings_json::GetJson(const std::string& fileKey) const
{
    const std::string& key = fileKey.empty() ? m_ActiveFileKey : fileKey;

    auto it = m_JsonFiles.find(key);
    if (it == m_JsonFiles.end())
    {
        throw std::runtime_error("[Error] 指定されたファイルキーが見つかりません: " + key);
    }

    return it->second.jsonContent;
}

nlohmann::json& Settings_json::GetJson(const std::string& fileKey)
{
    const std::string& key = fileKey.empty() ? m_ActiveFileKey : fileKey;

    auto it = m_JsonFiles.find(key);
    if (it == m_JsonFiles.end())
    {
        throw std::runtime_error("[Error] 指定されたファイルキーが見つかりません: " + key);
    }

    return it->second.jsonContent;
}

const nlohmann::json& Settings_json::GetValueFromKey(const nlohmann::json& json, const std::string& key) const
{
    static const nlohmann::json emptyJson; // 参照用の空JSONオブジェクト

    size_t delimiterPos = key.find('.');
    if (delimiterPos == std::string::npos)
    {
        return json.contains(key) ? json.at(key) : emptyJson;
    }

    std::string currentKey = key.substr(0, delimiterPos);
    std::string remainingKey = key.substr(delimiterPos + 1);

    if (!json.contains(currentKey) || !json[currentKey].is_object())
    {
        return emptyJson;
    }

    return GetValueFromKey(json.at(currentKey), remainingKey);
}

void Settings_json::NotifyChange(const std::string& key)
{
    for (const auto& callback : m_ChangeListeners)
    {
        callback(key);
    }
}

void Settings_json::AddChangeListener(SettingChangeCallback callback)
{
    m_ChangeListeners.push_back(std::move(callback));
}

void Settings_json::PrintAllSettings(const std::string& fileKey) const
{
    try
    {
        const auto& json = GetJson(fileKey);
        std::cout << json.dump(4) << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Settings_json::Save(const std::string& fileKey, const std::string& customFileName, bool overwrite)
{
    auto it = m_JsonFiles.find(fileKey);
    if (it == m_JsonFiles.end())
    {
        std::cerr << "[Error] 保存対象のファイルが見つかりません: " << fileKey << std::endl;
        return;
    }

    std::filesystem::path savePath = it->second.filePath;
    if (!customFileName.empty())
    {
        savePath = std::filesystem::path(m_OutputDirectory) / (customFileName + ".json");
    }

    if (std::filesystem::exists(savePath) && !overwrite)
    {
        int counter = 1;
        while (std::filesystem::exists(savePath))
        {
            savePath = std::filesystem::path(m_OutputDirectory) / (customFileName + "(" + std::to_string(counter++) + ").json");
        }
    }

    std::ofstream ofs(savePath);
    if (!ofs)
    {
        std::cerr << "[Error] ファイル保存に失敗しました: " << savePath << std::endl;
        return;
    }

    ofs << it->second.jsonContent.dump(4);
    std::cout << "[Info] 設定を保存しました: " << savePath << std::endl;
}

void Settings_json::SaveActiveFile()
{
    if (m_ActiveFileKey.empty())
    {
        std::cerr << "[Error] アクティブファイルが選択されていません。" << std::endl;
        return;
    }

    Save(m_ActiveFileKey);
}

void Settings_json::SaveAsActiveFile()
{
    if (m_ActiveFileKey.empty() || !m_JsonFiles.contains(m_ActiveFileKey)) return;

#ifdef _WIN32
    wchar_t filePath[MAX_PATH] = L"";

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameW(&ofn))
    {
        std::string savePath = WStringToString(filePath);
        Save(m_ActiveFileKey, savePath, true);
    }
#endif
}

void Settings_json::CheckFileChanges()
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    for (auto& [key, fileData] : m_JsonFiles)
    {
        auto currentTime = std::filesystem::last_write_time(fileData.filePath);
        if (m_FileModificationTimes[key] != currentTime)
        {
            std::cout << "[Info] ファイル変更検知: " << fileData.filePath << " - 再読み込み中..." << std::endl;
            LoadSettingJson(fileData.filePath.string(), key, false);
        }
    }
}

void Settings_json::StartFileWatcher()
{
    if (m_FileWatcherRunning) return;

    m_FileWatcherRunning = true;
    std::thread([this]() {
        while (m_FileWatcherRunning)
        {
            CheckFileChanges();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }).detach();
}

void Settings_json::StopFileWatcher()
{
    m_FileWatcherRunning = false;
}

void Settings_json::RenderJsonTree(nlohmann::json& json)
{
    for (auto& [key, value] : json.items())
    {
        if (value.is_object())
        {
            if (ImGui::TreeNode(key.c_str()))
            {
                RenderJsonTree(value); // 子要素の再帰表示
                ImGui::TreePop();
            }
        }
        else if (value.is_number_float())
        {
            float val = value.get<float>();
            if (ImGui::DragFloat(key.c_str(), &val, 0.1f))
            {
                value = val;
                NotifyChange(key);
            }
        }
        else if (value.is_number_integer())
        {
            int val = value.get<int>();
            if (ImGui::DragInt(key.c_str(), &val, 1))
            {
                value = val;
                NotifyChange(key);
            }
        }
        else if (value.is_boolean())
        {
            bool val = value.get<bool>();
            if (ImGui::Checkbox(key.c_str(), &val))
            {
                value = val;
                NotifyChange(key);
            }
        }
        else if (value.is_string())
        {
            std::string val = value.get<std::string>();
            char buffer[256] = { 0 };
            strncpy_s(buffer, val.c_str(), sizeof(buffer) - 1);

            if (ImGui::InputText(key.c_str(), buffer, sizeof(buffer)))
            {
                value = std::string(buffer);
                NotifyChange(key);
            }
        }
    }
}

void Settings_json::RenderImGuiEditor(const std::string& fileKey)
{
    std::string activeKey = fileKey.empty() ? m_ActiveFileKey : fileKey;

    if (activeKey.empty() || !m_JsonFiles.contains(activeKey))
    {
        ImGui::Text("No active file loaded.");
        return;
    }

    nlohmann::json& json = m_JsonFiles[activeKey].jsonContent;

    if (ImGui::Begin("JSON Settings Editor"))
    {
        RenderJsonTree(json);

        if (ImGui::Button("Save Settings"))
        {
            Save(activeKey);
        }
    }
    ImGui::End();
}

void Settings_json::RenderImGuiFileManager()
{
    if (ImGui::Begin("Settings File Manager", nullptr, ImGuiWindowFlags_MenuBar))
    {
        static char saveFileNameBuffer[128] = "";
        static bool overwrite = true;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open File"))
                {
                    OpenAndLoadFile();
                }

                if (!m_ActiveFileKey.empty())
                {
                    if (ImGui::MenuItem("Save (Overwrite)"))
                    {
                        Save(m_ActiveFileKey, WStringToString(m_JsonFiles[m_ActiveFileKey].filePath), true);
                    }

                    if (ImGui::MenuItem("Save As..."))
                    {
                        SaveAsActiveFile();
                    }

                    if (ImGui::MenuItem("Close Selected File"))
                    {
                        CloseFile(m_ActiveFileKey);
                    }
                }

                if (!m_JsonFiles.empty() && ImGui::MenuItem("Close All Files"))
                {
                    CloseAllFiles();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Separator();

        if (!m_JsonFiles.empty())
        {
            static int selectedFileIndex = 0;
            std::vector<std::string> keys;
            for (const auto& [key, _] : m_JsonFiles) keys.push_back(key);

            std::vector<const char*> keyCStrs;
            for (const auto& key : keys) keyCStrs.push_back(key.c_str());

            if (ImGui::Combo("Loaded Files", &selectedFileIndex, keyCStrs.data(), static_cast<int>(keyCStrs.size())))
            {
                m_ActiveFileKey = keys[selectedFileIndex];
            }
        }

        if (!m_ActiveFileKey.empty() && m_JsonFiles.contains(m_ActiveFileKey))
        {
            nlohmann::json& activeJson = m_JsonFiles[m_ActiveFileKey].jsonContent;

            ImGui::Separator();
            ImGui::Text("Active File: %s", m_ActiveFileKey.c_str());

            if (ImGui::BeginChild("Editor", ImVec2(0, 350), true))
            {
                RenderJsonTree(activeJson);
            }
            ImGui::EndChild();
        }
    }
    ImGui::End();
}



std::string Settings_json::OpenFileDialog()
{
#ifdef _WIN32
    wchar_t filePath[MAX_PATH] = L"";

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        return WStringToString(filePath);
    }
#endif
    return "";
}

void Settings_json::CloseFile(const std::string& fileKey)
{
    if (m_JsonFiles.contains(fileKey))
    {
        m_JsonFiles.erase(fileKey);
        if (m_ActiveFileKey == fileKey) m_ActiveFileKey.clear();
    }
}

void Settings_json::CloseAllFiles()
{
    m_JsonFiles.clear();
    m_ActiveFileKey.clear();
}

void Settings_json::OpenAndLoadFile()
{
#ifdef _WIN32
    wchar_t filePath[MAX_PATH] = L"";

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        std::string selectedFile = WStringToString(filePath);
        std::string fileKey = std::filesystem::path(selectedFile).stem().string();

        auto it = m_JsonFiles.find(fileKey);
        if (it == m_JsonFiles.end())
        {
            // 未読み込みの場合は自動で読み込み
            LoadSettingJson(selectedFile, fileKey, true);
            m_ActiveFileKey = fileKey;
        }
        else
        {
            // 既に読み込まれている場合はアクティブに設定
            m_ActiveFileKey = it->first;
        }
    }
#endif
}