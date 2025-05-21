#pragma once
#include "src/util/sound/SoundBase.h"
#include "src/util/sound/SoundDefine.h"

#include <unordered_map>
#include <vector>

using namespace SoundDefine;

/// <summary>
/// 音源の読み込み・再生・管理を行う
/// </summary>
namespace SoundManager {

	//================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// Updateの前に一度だけ呼び出される
	/// </summary>
	void Start();

	/// <summary>
	// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// カテゴリー別の音量倍率を適応させる
	/// </summary>
	void ApplyCategoryVolume();

	//================================================================================
	// ▼読み込み処理

	/// <summary>
	/// 音源を読み込む
	/// </summary>
	/// <param name="info">音源情報</param>
	void Load(const SoundInfo& info);

	/// <summary>
	/// Csvデータから音源情報を取得して読み込む
	/// </summary>
	/// <param name="filename">Csvデータが存在するファイルパス</param>
	void LoadToCsv(const std::string& filename);

	//================================================================================
	// ▼再生処理

	/// <summary>
	/// 音源を再生する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <returns>生成した音源の実体</returns>
	SoundBase* Play(const std::string& typeName, const std::string& label);

	/// <summary>
	/// 音源をランダムに再生する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <returns>生成した音源の実体</returns>
	SoundBase* PlayRandom(const std::vector<std::string>& typeName, const std::string& label);

	/// <summary>
	/// 音源を立体音響で再生する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="playPos">再生座標</param>
	/// <returns>生成した音源の実体</returns>
	SoundBase* PlaySetPan(const std::string& typeName, const std::string& label, Vector3* playPos);

	/// <summary>
	/// 音源の生成座標を設定する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="playPos">再生座標</param>
	void SetPan(const std::string& typeName, const std::string& label, Vector3* playPos);

	/// <summary>
	/// 音源の周波数を変更して再生する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="frequency">再生周波数(ヘルツ単位。100～100,000 : -1でデフォルトに戻す)</param>
	/// <remarks>
	/// 値が小さいほど、遅く低く、値が大きいほど速く高く再生されます。
	/// 音の元の再生周波数が22.05KHzだった場合は【22050】を指定した場合が通常の再生値となり、
	/// 44.10KHzだった場合は【44100】が通常の再生値となる。
	/// </remarks>
	/// <returns>生成した音源の実体</returns>
	SoundBase* PlaySetFrequency(const std::string& typeName, const std::string& label, const float& frequency);

	/// <summary>
	/// 音源の周波数を設定する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="frequency">再生周波数(ヘルツ単位。100～100,000 : -1でデフォルトに戻す)</param>
	void SetFrequency(const std::string& typeName, const std::string& label, const float& frequency);

	/// <summary>
	/// 音源の音量を徐々に上げながら再生する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="sec">効果時間(秒)</param>
	/// <param name="easing">補間方法の種類</param>
	/// <returns>生成した音源の実体</returns>
	SoundBase* FadeIn(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing = EasingType::Linear);

	/// <summary>
	/// 音源の音量を徐々に下げる
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <param name="sec">効果時間(秒)</param>
	/// <param name="easing">補間方法の種類</param>
	/// <param name="isFadeOutEnd">音量が0になった時に、音源の再生を終了するか</param>
	/// <returns>生成した音源の実体</returns>
	SoundBase* FadeOut(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing = EasingType::Linear, const bool& isFadeOutEnd = true);

	/// <summary>
	/// 音源が再生されているか
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	/// <returns>再生されていた場合、その実体を返す。再生されていなければnullptrを返す。</returns>
	SoundBase* IsPlaying(const std::string& typeName, const std::string& label);

	//================================================================================
	// ▼停止処理

	/// <summary>
	/// 音源を停止する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <param name="label">判別名</param>
	void Stop(const std::string& typeName, const std::string& label);

	/// <summary>
	/// 分類ごとの音源を停止する
	/// </summary>
	/// <param name="category">停止する音源の分類</param>
	void StopCategory(const SoundCategory& category);

	/// <summary>
	/// 全ての音源を停止する
	/// </summary>
	void StopAll();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 分類ごとの音量を設定する
	/// </summary>
	/// <param name="category">設定する音源の分類</param>
	/// <param name="rate">音量の倍率(0..1)</param>
	void SetVolumeRate(const SoundCategory& category, const float& rate);

	//================================================================================
	// ▼ゲッター

	std::unordered_map<SoundCategory, float>* CategoryVolumeRate();

	//================================================================================
	// ▼確認処理

	/// <summary>
	/// データが読み込まれているかを判定する
	/// </summary>
	/// <param name="typeName">音源のデータ名</param>
	/// <returns>読み込まれていた場合、true。</returns>
	bool CheckLoadDate(const std::string& typeName);

	//================================================================================
	// ▼データ解放処理

	/// <summary>
	/// Csvデータから再度読み込みを行う
	/// </summary>
	void Reload();

	/// <summary>
	/// 全ての音源データを解放する
	/// </summary>
	void AllReleaseInfo();

#ifdef IMGUI
	
	/// <summary>
	/// デバッグで使用するImGui関連の機能の初期化を行う(読み込んだサウンドのプレビューを行う)
	/// </summary>
	void InitImGui();

#endif // IMGUI

};