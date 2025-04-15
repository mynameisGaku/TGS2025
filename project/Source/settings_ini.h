// settings_ini.h
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include <filesystem>

/// <summary>
/// INI �ݒ�t�@�C����ǂݍ��݁A�ۑ��A�擾�A�Ď�������N���X�B
/// CSVReader�Ƒ�������
/// </summary>
class Settings_ini
{
public:
    /// <summary>
    /// �ݒ�ύX���ɌĂяo�����R�[���o�b�N�֐��^�B
    /// </summary>
    using ChangeCallback = std::function<void(const std::string& key)>;

    /// <summary>
    /// �V���O���g���C���X�^���X���擾���܂��B
    /// </summary>
    /// <returns>Settings_ini �̃C���X�^���X</returns>
    static Settings_ini& Inst();

    /// <summary>
    /// INI �t�@�C����ǂݍ��݂܂��B
    /// </summary>
    /// <param name="filepath">�ǂݍ��ރt�@�C���̃p�X</param>
    /// <param name="key">�t�@�C�������ʂ���L�[</param>
    /// <param name="setActive">�ǂݍ��񂾃t�@�C�����A�N�e�B�u�ɂ��邩�ǂ���</param>
    void Load(const std::string& filepath, const std::string& key, bool setActive = false);

    /// <summary>
    /// �w��L�[�̒l���擾���܂��B
    /// </summary>
    /// <typeparam name="T">�擾����l�̌^</typeparam>
    /// <param name="key">�擾�Ώۂ̃L�[</param>
    /// <param name="fileKey">�Ώۃt�@�C���̃L�[�i�ȗ��j</param>
    /// <returns>�w�肳�ꂽ�^�̒l</returns>
    template<typename T>
    T Get(const std::string& key, const std::string& fileKey = "") const
    {
        const auto& values = GetFileValues(fileKey);
        auto it = values.find(key);

        if (it == values.end())
        {
            throw std::runtime_error("[�G���[] �w��L�[�����݂��܂���: " + key);
        }

        std::istringstream iss(it->second);
        T result;
        iss >> result;

        if (iss.fail())
        {
            throw std::runtime_error("[�G���[] �l�̕ϊ��Ɏ��s���܂���: " + it->second);
        }

        return result;
    }

    /// <summary>
    /// �w��L�[�ɒl��ݒ肵�܂��B
    /// </summary>
    /// <typeparam name="T">�ݒ肷��l�̌^</typeparam>
    /// <param name="key">�ݒ�Ώۂ̃L�[</param>
    /// <param name="value">�ݒ肷��l</param>
    /// <param name="fileKey">�Ώۃt�@�C���̃L�[�i�ȗ��j</param>
    template<typename T>
    void Set(const std::string& key, const T& value, const std::string& fileKey = "")
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::ostringstream oss;
            oss << value;
            iniFiles_[fileKey.empty() ? activeFile_ : fileKey][key] = oss.str();
        }

        NotifyChange(key);
    }

    /// <summary>
    /// INI �ݒ���t�@�C���ɕۑ����܂��B
    /// </summary>
    /// <param name="filepath">�ۑ���t�@�C���̃p�X</param>
    /// <param name="fileKey">�Ώۃt�@�C���̃L�[�i�ȗ��j</param>
    void Save(const std::string& filepath, const std::string& fileKey = "");

    /// <summary>
    /// �ݒ�ύX���X�i�[��o�^���܂��B
    /// </summary>
    /// <param name="callback">�ύX���ɌĂяo�����R�[���o�b�N�֐�</param>
    void AddChangeListener(ChangeCallback callback);

    /// <summary>
    /// �t�@�C���ύX�Ď����J�n���܂��B
    /// </summary>
    void StartWatcher();

    /// <summary>
    /// �t�@�C���ύX�Ď����~���܂��B
    /// </summary>
    void StopWatcher();

    /// <summary>
    /// ImGui ���g�p���Đݒ�����A���^�C���ҏW���܂��B
    /// </summary>
    /// <param name="fileKey">�Ώۃt�@�C���̃L�[�i�ȗ��j</param>
    void RenderImGui(const std::string& fileKey = "");

private:
    /// <summary>
    /// �R���X�g���N�^�i�V���O���g���̂��� private�j�B
    /// </summary>
    Settings_ini();

    /// <summary>
    /// �f�X�g���N�^�B
    /// </summary>
    ~Settings_ini();

    /// <summary>
    /// �ݒ�ύX�����X�i�[�ɒʒm���܂��B
    /// </summary>
    /// <param name="key">�ύX���ꂽ�L�[</param>
    void NotifyChange(const std::string& key);

    /// <summary>
    /// �t�@�C���ύX���Ď����A�ύX�����m�����ꍇ�ɍēǂݍ��݂��܂��B
    /// </summary>
    void WatchLoop();

    /// <summary>
    /// �w��t�@�C���̒l�}�b�v���擾���܂��B
    /// </summary>
    /// <param name="key">�Ώۃt�@�C���̃L�[</param>
    /// <returns>�L�[�ƒl�̃}�b�v</returns>
    const std::unordered_map<std::string, std::string>& GetFileValues(const std::string& key) const;

    /// <summary>
    /// ������̑O��̋󔒂��폜���܂��B
    /// </summary>
    /// <param name="s">�Ώۂ̕�����</param>
    /// <returns>�g�������ꂽ������</returns>
    static std::string Trim(const std::string& s);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> iniFiles_;  ///< �t�@�C�����Ƃ̃L�[�ƒl�̃}�b�v
    std::unordered_map<std::string, std::filesystem::file_time_type> modificationTimes_;      ///< �t�@�C���̍ŏI�ύX���Ԃ̋L�^
    std::vector<ChangeCallback> listeners_;                                                  ///< �ݒ�ύX���X�i�[�̃��X�g
    std::string activeFile_;                                                                  ///< ���݃A�N�e�B�u�ȃt�@�C���̃L�[
    bool watcherRunning_ = false;                                                             ///< �t�@�C���E�H�b�`���[���s�t���O
    mutable std::mutex mutex_;                                                                ///< �r������p�~���[�e�b�N�X
};

// �����I�ȃe���v���[�g���ꉻ�� settings_ini.cpp �ɋL�q

// �ݒ���擾
template<typename T>
T Ini_GetValue(const std::string& key, const std::string& file)
{
    return Settings_ini::Inst().Get<T>(key, file);
}
