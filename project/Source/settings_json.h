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

//=== 追加のJson変換 ===
#include "VectorJson.h"

/// <summary>
/// JSON設定ファイルの読み込み、保存、取得、監視、及びインゲームでのImGuiによるリアルタイム変更を行うクラス。
/// CSVReaderと相性悪い
/// </summary>
class Settings_json
{
private:
    /// <summary>
    /// ファイルごとのJSONデータおよび関連情報を保持する構造体。
    /// </summary>
    struct FileData
    {
        nlohmann::json jsonContent; // 読み込んだJSONデータ。
        std::filesystem::path filePath; // 対応するファイルパス。
    };

    static Settings_json* m_Inst; // シングルトンインスタンス。

    std::unordered_map<std::string, FileData> m_JsonFiles; // fileKey -> ファイルデータ。
    std::unordered_map<std::string, std::filesystem::file_time_type> m_FileModificationTimes; // ファイル変更時間の管理用マップ。
    std::vector<std::function<void(const std::string&)>> m_ChangeListeners; // 設定変更リスナー群。

    std::mutex m_Mutex; // スレッド安全用ミューテックス。
    std::string m_ActiveFileKey; // 現在アクティブなファイルキー。
    std::string m_OutputDirectory = "data/json/"; // ファイル保存先ディレクトリ。

    bool m_FileWatcherRunning = false; // ファイルウォッチャー実行フラグ。

    Settings_json() = default; // プライベートコンストラクタ (シングルトン用)。

public:
    using SettingChangeCallback = std::function<void(const std::string& key)>;

    /// <summary>
    /// シングルトンインスタンスを取得します。
    /// </summary>
    static Settings_json* Inst();

    /// <summary>
    /// インスタンスを破棄します。ウォッチャー停止とメモリ解放を実行。
    /// </summary>
    static void Destroy();

    /// <summary>
    /// 出力先ディレクトリを設定します。
    /// 指定したディレクトリが存在しない場合は自動で作成。
    /// </summary>
    /// <param name="directory">新しい出力ディレクトリパス。</param>
    void SetOutputDirectory(const std::string& directory);

    /// <summary>
    /// JSONファイルを読み込み、指定キーで管理します。
    /// </summary>
    /// <param name="filepath">読み込むファイルのパス。</param>
    /// <param name="fileKey">管理用のファイルキー。</param>
    /// <param name="setActive">読み込み後にアクティブファイルに設定するかどうか。</param>
    void LoadSettingJson(const std::string& filepath, const std::string& fileKey, bool setActive = false);

    /// <summary>
    /// 指定ファイルが他で開かれているか確認します。
    /// </summary>
    /// <param name="filepath">確認するファイルパス。</param>
    /// <returns>使用中の場合はtrue。</returns>
    bool IsFileInUse(const std::string& filepath) const;

    /// <summary>
    /// 指定キーの値を取得します。
    /// </summary>
    /// <typeparam name="T">取得する型。</typeparam>
    /// <param name="key">対象キー (ネスト構造時はドット区切り)。</param>
    /// <param name="fileKey">対象ファイルキー (未指定時はアクティブファイル)。</param>
    /// <returns>指定キーの値。</returns>
    template<typename T>
    inline T Get(const std::string& key, const std::string& fileKey = "") const
    {
        const nlohmann::json& json = GetJson(fileKey);
        return Convert<T>(GetValueFromKey(json, key));
    }

    /// <summary>
    /// 指定キーの値を取得します。存在しない場合はデフォルト値を返します。
    /// </summary>
    /// <typeparam name="T">取得する型。</typeparam>
    /// <param name="key">対象キー。</param>
    /// <param name="defaultValue">存在しない場合のデフォルト値。</param>
    /// <param name="fileKey">対象ファイルキー (未指定時はアクティブファイル)。</param>
    /// <returns>値またはデフォルト値。</returns>
    template<typename T>
    inline T GetOrDefault(const std::string& key, const T& defaultValue, const std::string& fileKey = "") const
    {
        const nlohmann::json& json = GetJson(fileKey);
        const nlohmann::json& value = GetValueFromKey(json, key);
        return value.is_null() ? defaultValue : Convert<T>(value);
    }

    /// <summary>
    /// 指定キーに値を設定します。
    /// </summary>
    /// <typeparam name="T">設定する値の型。</typeparam>
    /// <param name="key">対象キー。</param>
    /// <param name="value">設定値。</param>
    /// <param name="fileKey">対象ファイルキー (未指定時はアクティブファイル)。</param>
    template<typename T>
    inline void Set(const std::string& key, const T& value, const std::string& fileKey = "")
    {
        nlohmann::json& json = GetJson(fileKey);
        SetValueFromKey(json, key, value);
        NotifyChange(key);
    }

    /// <summary>
    /// 設定変更時のリスナーを追加します。
    /// </summary>
    /// <param name="callback">変更時に呼ばれる関数。</param>
    void AddChangeListener(SettingChangeCallback callback);

    /// <summary>
    /// ファイル監視を開始します。ファイル変更時に自動再読み込み。
    /// </summary>
    void StartFileWatcher();

    /// <summary>
    /// ファイル監視を停止します。
    /// </summary>
    void StopFileWatcher();

    /// <summary>
    /// 設定内容をコンソールに出力 (デバッグ用)。
    /// </summary>
    /// <param name="fileKey">対象ファイルキー。</param>
    void PrintAllSettings(const std::string& fileKey = "") const;

    /// <summary>
    /// 指定ファイルを保存します。
    /// </summary>
    /// <param name="fileKey">保存対象ファイルキー。</param>
    /// <param name="customFileName">保存先ファイル名 (空欄時はfileKeyを使用)。</param>
    /// <param name="overwrite">既存ファイル上書き可否。</param>
    void Save(const std::string& fileKey, const std::string& customFileName = "", bool overwrite = false);

    /// <summary>
    /// アクティブファイルを現在のパスに上書き保存。
    /// </summary>
    void SaveActiveFile();

    /// <summary>
    /// アクティブファイルを別名で保存します。
    /// </summary>
    void SaveAsActiveFile();

    /// <summary>
    /// ImGuiエディタを表示して設定ファイルを編集可能にします。
    /// </summary>
    /// <param name="fileKey">対象ファイルキー (未指定時はアクティブファイル)。</param>
    void RenderImGuiEditor(const std::string& fileKey = "");

    /// <summary>
    /// ImGuiファイルマネージャを表示します。
    /// </summary>
    void RenderImGuiFileManager();

    std::string OpenFileDialog();

    /// <summary>
    /// 指定されたファイルキーに対応する設定を閉じます。
    /// </summary>
    void CloseFile(const std::string& fileKey);

    /// <summary>
    /// すべてのロード済み設定ファイルを閉じます。
    /// </summary>
    void CloseAllFiles();

    /// <summary>
    /// ファイル選択ダイアログを表示して選択されたファイルパスを取得し、選択されたファイルを即時読み込みます。
    /// </summary>
    void OpenAndLoadFile();


    /// <summary>
    /// JSONデータを取得 (const版)。
    /// </summary>
    /// <param name="fileKey">対象ファイルキー。</param>
    /// <returns>JSONデータへの参照。</returns>
    const nlohmann::json& GetJson(const std::string& fileKey) const;

    /// <summary>
    /// JSONデータを取得 (非const版)。
    /// </summary>
    /// <param name="fileKey">対象ファイルキー。</param>
    /// <returns>JSONデータへの参照。</returns>
    nlohmann::json& GetJson(const std::string& fileKey);

private:

    /// <summary>
    /// JSON内の指定キーの値を取得。
    /// </summary>
    /// <param name="json">対象JSON。</param>
    /// <param name="key">ドット区切りのキー。</param>
    /// <returns>該当値の参照。</returns>
    const nlohmann::json& GetValueFromKey(const nlohmann::json& json, const std::string& key) const;

    /// <summary>
    /// JSON内の指定キーに値を設定。
    /// </summary>
    /// <typeparam name="T">設定する値の型。</typeparam>
    /// <param name="json">対象JSON。</param>
    /// <param name="key">キー。</param>
    /// <param name="value">設定値。</param>
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
    /// 設定変更リスナーに通知。
    /// </summary>
    /// <param name="key">変更されたキー。</param>
    void NotifyChange(const std::string& key);

    /// <summary>
    /// ファイル変更確認と再読み込み。
    /// </summary>
    void CheckFileChanges();

    /// <summary>
    /// ImGuiでJSONツリーを表示・編集。
    /// </summary>
    /// <param name="json">対象JSON。</param>
    void RenderJsonTree(nlohmann::json& json);

    /// <summary>
    /// JSON値を指定型に変換。
    /// </summary>
    /// <typeparam name="T">変換先型。</typeparam>
    /// <param name="value">対象値。</param>
    /// <returns>変換済み値。</returns>
    template<typename T>
    static inline T Convert(const nlohmann::json& value)
    {
        return value.is_null() ? T{} : value.get<T>();
    }
};
