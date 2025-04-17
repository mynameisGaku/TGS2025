#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <functional>
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <mutex>
#include <nlohmann/json.hpp>

#include "../vendor/ImGui/imgui.h"

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

//=== �ǉ���Json�ϊ� ===
#include "VectorJson.h"

/// <summary>
/// JSON�ݒ�t�@�C���̓ǂݍ��݁A�ۑ��A�擾�A�Ď��A�y�уC���Q�[���ł�ImGui�ɂ�郊�A���^�C���ύX���s���N���X�B
/// CSVReader�Ƒ�������
/// </summary>
class Settings_json
{
private:
    /// <summary>
    /// �t�@�C�����Ƃ�JSON�f�[�^����ъ֘A����ێ�����\���́B
    /// </summary>
    struct FileData
    {
        nlohmann::json jsonContent; // �ǂݍ���JSON�f�[�^�B
        std::filesystem::path filePath; // �Ή�����t�@�C���p�X�B
    };

    static Settings_json* m_Inst; // �V���O���g���C���X�^���X�B

    std::unordered_map<std::string, FileData> m_JsonFiles; // fileKey -> �t�@�C���f�[�^�B
    std::unordered_map<std::string, std::filesystem::file_time_type> m_FileModificationTimes; // �t�@�C���ύX���Ԃ̊Ǘ��p�}�b�v�B
    std::vector<std::function<void(const std::string&)>> m_ChangeListeners; // �ݒ�ύX���X�i�[�Q�B

    std::mutex m_Mutex; // �X���b�h���S�p�~���[�e�b�N�X�B
    std::string m_ActiveFileKey; // ���݃A�N�e�B�u�ȃt�@�C���L�[�B
    std::string m_OutputDirectory = "data/json/"; // �t�@�C���ۑ���f�B���N�g���B

    bool m_FileWatcherRunning = false; // �t�@�C���E�H�b�`���[���s�t���O�B

    Settings_json() = default; // �v���C�x�[�g�R���X�g���N�^ (�V���O���g���p)�B

public:
    using SettingChangeCallback = std::function<void(const std::string& key)>;

    /// <summary>
    /// �V���O���g���C���X�^���X���擾���܂��B
    /// </summary>
    static Settings_json* Inst();

    /// <summary>
    /// �C���X�^���X��j�����܂��B�E�H�b�`���[��~�ƃ�������������s�B
    /// </summary>
    static void Destroy();

    /// <summary>
    /// �o�͐�f�B���N�g����ݒ肵�܂��B
    /// �w�肵���f�B���N�g�������݂��Ȃ��ꍇ�͎����ō쐬�B
    /// </summary>
    /// <param name="directory">�V�����o�̓f�B���N�g���p�X�B</param>
    void SetOutputDirectory(const std::string& directory);

    /// <summary>
    /// JSON�t�@�C����ǂݍ��݁A�w��L�[�ŊǗ����܂��B
    /// </summary>
    /// <param name="filepath">�ǂݍ��ރt�@�C���̃p�X�B</param>
    /// <param name="fileKey">�Ǘ��p�̃t�@�C���L�[�B</param>
    /// <param name="setActive">�ǂݍ��݌�ɃA�N�e�B�u�t�@�C���ɐݒ肷�邩�ǂ����B</param>
    void LoadSettingJson(const std::string& filepath, const std::string& fileKey, bool setActive = false);

    /// <summary>
    /// �w��t�@�C�������ŊJ����Ă��邩�m�F���܂��B
    /// </summary>
    /// <param name="filepath">�m�F����t�@�C���p�X�B</param>
    /// <returns>�g�p���̏ꍇ��true�B</returns>
    bool IsFileInUse(const std::string& filepath) const;

    /// <summary>
    /// �w��L�[�̒l���擾���܂��B
    /// </summary>
    /// <typeparam name="T">�擾����^�B</typeparam>
    /// <param name="key">�ΏۃL�[ (�l�X�g�\�����̓h�b�g��؂�)�B</param>
    /// <param name="fileKey">�Ώۃt�@�C���L�[ (���w�莞�̓A�N�e�B�u�t�@�C��)�B</param>
    /// <returns>�w��L�[�̒l�B</returns>
    template<typename T>
    inline T Get(const std::string& key, const std::string& fileKey = "") const
    {
        const nlohmann::json& json = GetJson(fileKey);
        return Convert<T>(GetValueFromKey(json, key));
    }

    /// <summary>
    /// �w��L�[�̒l���擾���܂��B���݂��Ȃ��ꍇ�̓f�t�H���g�l��Ԃ��܂��B
    /// </summary>
    /// <typeparam name="T">�擾����^�B</typeparam>
    /// <param name="key">�ΏۃL�[�B</param>
    /// <param name="defaultValue">���݂��Ȃ��ꍇ�̃f�t�H���g�l�B</param>
    /// <param name="fileKey">�Ώۃt�@�C���L�[ (���w�莞�̓A�N�e�B�u�t�@�C��)�B</param>
    /// <returns>�l�܂��̓f�t�H���g�l�B</returns>
    template<typename T>
    inline T GetOrDefault(const std::string& key, const T& defaultValue, const std::string& fileKey = "") const
    {
        const nlohmann::json& json = GetJson(fileKey);
        const nlohmann::json& value = GetValueFromKey(json, key);
        return value.is_null() ? defaultValue : Convert<T>(value);
    }

    /// <summary>
    /// �w��L�[�ɒl��ݒ肵�܂��B
    /// </summary>
    /// <typeparam name="T">�ݒ肷��l�̌^�B</typeparam>
    /// <param name="key">�ΏۃL�[�B</param>
    /// <param name="value">�ݒ�l�B</param>
    /// <param name="fileKey">�Ώۃt�@�C���L�[ (���w�莞�̓A�N�e�B�u�t�@�C��)�B</param>
    template<typename T>
    inline void Set(const std::string& key, const T& value, const std::string& fileKey = "")
    {
        nlohmann::json& json = GetJson(fileKey);
        SetValueFromKey(json, key, value);
        NotifyChange(key);
    }

    /// <summary>
    /// �ݒ�ύX���̃��X�i�[��ǉ����܂��B
    /// </summary>
    /// <param name="callback">�ύX���ɌĂ΂��֐��B</param>
    void AddChangeListener(SettingChangeCallback callback);

    /// <summary>
    /// �t�@�C���Ď����J�n���܂��B�t�@�C���ύX���Ɏ����ēǂݍ��݁B
    /// </summary>
    void StartFileWatcher();

    /// <summary>
    /// �t�@�C���Ď����~���܂��B
    /// </summary>
    void StopFileWatcher();

    /// <summary>
    /// �ݒ���e���R���\�[���ɏo�� (�f�o�b�O�p)�B
    /// </summary>
    /// <param name="fileKey">�Ώۃt�@�C���L�[�B</param>
    void PrintAllSettings(const std::string& fileKey = "") const;

    /// <summary>
    /// �w��t�@�C����ۑ����܂��B
    /// </summary>
    /// <param name="fileKey">�ۑ��Ώۃt�@�C���L�[�B</param>
    /// <param name="customFileName">�ۑ���t�@�C���� (�󗓎���fileKey���g�p)�B</param>
    /// <param name="overwrite">�����t�@�C���㏑���ہB</param>
    void Save(const std::string& fileKey, const std::string& customFileName = "", bool overwrite = false);

    /// <summary>
    /// �A�N�e�B�u�t�@�C�������݂̃p�X�ɏ㏑���ۑ��B
    /// </summary>
    void SaveActiveFile();

    /// <summary>
    /// �A�N�e�B�u�t�@�C����ʖ��ŕۑ����܂��B
    /// </summary>
    void SaveAsActiveFile();

    /// <summary>
    /// ImGui�G�f�B�^��\�����Đݒ�t�@�C����ҏW�\�ɂ��܂��B
    /// </summary>
    /// <param name="fileKey">�Ώۃt�@�C���L�[ (���w�莞�̓A�N�e�B�u�t�@�C��)�B</param>
    void RenderImGuiEditor(const std::string& fileKey = "");

    /// <summary>
    /// ImGui�t�@�C���}�l�[�W����\�����܂��B
    /// </summary>
    void RenderImGuiFileManager();

    std::string OpenFileDialog();

    /// <summary>
    /// �w�肳�ꂽ�t�@�C���L�[�ɑΉ�����ݒ����܂��B
    /// </summary>
    void CloseFile(const std::string& fileKey);

    /// <summary>
    /// ���ׂẴ��[�h�ςݐݒ�t�@�C������܂��B
    /// </summary>
    void CloseAllFiles();

    /// <summary>
    /// �t�@�C���I���_�C�A���O��\�����đI�����ꂽ�t�@�C���p�X���擾���A�I�����ꂽ�t�@�C���𑦎��ǂݍ��݂܂��B
    /// </summary>
    void OpenAndLoadFile();


    /// <summary>
    /// JSON�f�[�^���擾 (const��)�B
    /// </summary>
    /// <param name="fileKey">�Ώۃt�@�C���L�[�B</param>
    /// <returns>JSON�f�[�^�ւ̎Q�ƁB</returns>
    const nlohmann::json& GetJson(const std::string& fileKey) const;

    /// <summary>
    /// JSON�f�[�^���擾 (��const��)�B
    /// </summary>
    /// <param name="fileKey">�Ώۃt�@�C���L�[�B</param>
    /// <returns>JSON�f�[�^�ւ̎Q�ƁB</returns>
    nlohmann::json& GetJson(const std::string& fileKey);

private:

    /// <summary>
    /// JSON���̎w��L�[�̒l���擾�B
    /// </summary>
    /// <param name="json">�Ώ�JSON�B</param>
    /// <param name="key">�h�b�g��؂�̃L�[�B</param>
    /// <returns>�Y���l�̎Q�ƁB</returns>
    const nlohmann::json& GetValueFromKey(const nlohmann::json& json, const std::string& key) const;

    /// <summary>
    /// JSON���̎w��L�[�ɒl��ݒ�B
    /// </summary>
    /// <typeparam name="T">�ݒ肷��l�̌^�B</typeparam>
    /// <param name="json">�Ώ�JSON�B</param>
    /// <param name="key">�L�[�B</param>
    /// <param name="value">�ݒ�l�B</param>
    template<typename T>
    void SetValueFromKey(nlohmann::json& json, const std::string& key, const T& value)
    {
        size_t delimiterPos = key.find('.');
        if (delimiterPos == std::string::npos)
        {
            json[key] = value;
            return;
        }

        std::string currentKey = key.substr(0, delimiterPos);
        std::string remainingKey = key.substr(delimiterPos + 1);

        if (!json.contains(currentKey) || !json[currentKey].is_object())
        {
            json[currentKey] = nlohmann::json::object();
        }

        SetValueFromKey(json[currentKey], remainingKey, value);
    }

    /// <summary>
    /// �ݒ�ύX���X�i�[�ɒʒm�B
    /// </summary>
    /// <param name="key">�ύX���ꂽ�L�[�B</param>
    void NotifyChange(const std::string& key);

    /// <summary>
    /// �t�@�C���ύX�m�F�ƍēǂݍ��݁B
    /// </summary>
    void CheckFileChanges();

    /// <summary>
    /// ImGui��JSON�c���[��\���E�ҏW�B
    /// </summary>
    /// <param name="json">�Ώ�JSON�B</param>
    void RenderJsonTree(nlohmann::json& json);

    /// <summary>
    /// JSON�l���w��^�ɕϊ��B
    /// </summary>
    /// <typeparam name="T">�ϊ���^�B</typeparam>
    /// <param name="value">�Ώےl�B</param>
    /// <returns>�ϊ��ςݒl�B</returns>
    template<typename T>
    static inline T Convert(const nlohmann::json& value)
    {
        return value.is_null() ? T{} : value.get<T>();
    }
};
