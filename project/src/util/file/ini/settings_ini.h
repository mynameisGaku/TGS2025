// settings_ini.h
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include <filesystem>

/// <summary>
/// INI 設定ファイルを読み込み、保存、取得、監視をするクラス。
/// CSVReaderと相性悪い
/// </summary>
class Settings_ini
{
public:
    /// <summary>
    /// 設定変更時に呼び出されるコールバック関数型。
    /// </summary>
    using ChangeCallback = std::function<void(const std::string& key)>;

    /// <summary>
    /// シングルトンインスタンスを取得します。
    /// </summary>
    /// <returns>Settings_ini のインスタンス</returns>
    static Settings_ini& Inst();

    /// <summary>
    /// INI ファイルを読み込みます。
    /// </summary>
    /// <param name="filepath">読み込むファイルのパス</param>
    /// <param name="key">ファイルを識別するキー</param>
    /// <param name="setActive">読み込んだファイルをアクティブにするかどうか</param>
    void Load(const std::string& filepath, const std::string& key, bool setActive = false);

    /// <summary>
    /// 指定キーの値を取得します。
    /// </summary>
    /// <typeparam name="T">取得する値の型</typeparam>
    /// <param name="key">取得対象のキー</param>
    /// <param name="fileKey">対象ファイルのキー（省略可）</param>
    /// <returns>指定された型の値</returns>
    template<typename T>
    T Get(const std::string& key, const std::string& fileKey = "") const
    {
        const auto& values = GetFileValues(fileKey);
        auto it = values.find(key);

        if (it == values.end())
        {
            throw std::runtime_error("[エラー] 指定キーが存在しません: " + key);
        }

        std::istringstream iss(it->second);
        T result;
        iss >> result;

        if (iss.fail())
        {
            throw std::runtime_error("[エラー] 値の変換に失敗しました: " + it->second);
        }

        return result;
    }

    /// <summary>
    /// 指定キーに値を設定します。
    /// </summary>
    /// <typeparam name="T">設定する値の型</typeparam>
    /// <param name="key">設定対象のキー</param>
    /// <param name="value">設定する値</param>
    /// <param name="fileKey">対象ファイルのキー（省略可）</param>
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
    /// INI 設定をファイルに保存します。
    /// </summary>
    /// <param name="filepath">保存先ファイルのパス</param>
    /// <param name="fileKey">対象ファイルのキー（省略可）</param>
    void Save(const std::string& filepath, const std::string& fileKey = "");

    /// <summary>
    /// 設定変更リスナーを登録します。
    /// </summary>
    /// <param name="callback">変更時に呼び出されるコールバック関数</param>
    void AddChangeListener(ChangeCallback callback);

    /// <summary>
    /// ファイル変更監視を開始します。
    /// </summary>
    void StartWatcher();

    /// <summary>
    /// ファイル変更監視を停止します。
    /// </summary>
    void StopWatcher();

    /// <summary>
    /// ImGui を使用して設定をリアルタイム編集します。
    /// </summary>
    /// <param name="fileKey">対象ファイルのキー（省略可）</param>
    void RenderImGui(const std::string& fileKey = "");

private:
    /// <summary>
    /// コンストラクタ（シングルトンのため private）。
    /// </summary>
    Settings_ini();

    /// <summary>
    /// デストラクタ。
    /// </summary>
    ~Settings_ini();

    /// <summary>
    /// 設定変更をリスナーに通知します。
    /// </summary>
    /// <param name="key">変更されたキー</param>
    void NotifyChange(const std::string& key);

    /// <summary>
    /// ファイル変更を監視し、変更を検知した場合に再読み込みします。
    /// </summary>
    void WatchLoop();

    /// <summary>
    /// 指定ファイルの値マップを取得します。
    /// </summary>
    /// <param name="key">対象ファイルのキー</param>
    /// <returns>キーと値のマップ</returns>
    const std::unordered_map<std::string, std::string>& GetFileValues(const std::string& key) const;

    /// <summary>
    /// 文字列の前後の空白を削除します。
    /// </summary>
    /// <param name="s">対象の文字列</param>
    /// <returns>トリムされた文字列</returns>
    static std::string Trim(const std::string& s);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> iniFiles_;  ///< ファイルごとのキーと値のマップ
    std::unordered_map<std::string, std::filesystem::file_time_type> modificationTimes_;      ///< ファイルの最終変更時間の記録
    std::vector<ChangeCallback> listeners_;                                                  ///< 設定変更リスナーのリスト
    std::string activeFile_;                                                                  ///< 現在アクティブなファイルのキー
    bool watcherRunning_ = false;                                                             ///< ファイルウォッチャー実行フラグ
    mutable std::mutex mutex_;                                                                ///< 排他制御用ミューテックス
};

// 明示的なテンプレート特殊化は settings_ini.cpp に記述

// 設定を取得
template<typename T>
T Ini_GetValue(const std::string& key, const std::string& file)
{
    return Settings_ini::Inst().Get<T>(key, file);
}
