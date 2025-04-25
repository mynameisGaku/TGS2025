#pragma once
#include "EffectBase.h"
#include "EffectDefine.h"

#include "config.h"

using namespace EffectDefine;

/// <summary>
/// エフェクトの読み込み・再生・管理を行う
/// </summary>
namespace EffectManager {

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
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	//================================================================================
	// ▼読み込み処理

	/// <summary>
	/// エフェクトを読み込む
	/// </summary>
	/// <param name="info">エフェクト情報</param>
	void Load(const EffectInfo& info);

	/// <summary>
	/// Csvデータからエフェクト情報を取得して読み込む
	/// </summary>
	/// <param name="filename">Csvデータが存在するファイルパス</param>
	void LoadToCsv(const std::string& filename);

	/// <summary>
	/// Jsonからエフェクト情報を取得して読み込む
	/// </summary>
	/// <param name="filename">Jsonのファイルパス</param>
	void LoadFromJson(const std::string& filename);

	//================================================================================
	// ▼再生処理

	/// <summary>
	/// エフェクトを3D空間上に生成する
	/// </summary>
	/// <param name="typeName">エフェクトのデータ名</param>
	/// <param name="trs">座標・回転・拡縮の情報</param>
	/// <param name="label">判別名</param>
	/// <param name="isLoop">ループ再生を行うか</param>
	/// <returns>生成したエフェクトの実体</returns>
	EffectBase* Play3D(const std::string& typeName, const Transform& trs, const std::string& label, const bool& isLoop = false);

	/// <summary>
	/// エフェクトを2D空間上に生成する
	/// </summary>
	/// <param name="typeName">エフェクトのデータ名</param>
	/// <param name="trs">座標・回転・拡縮の情報</param>
	/// <param name="label">判別名</param>
	/// <param name="isLoop">ループ再生を行うか</param>
	/// <returns>生成したエフェクトの実体</returns>
	EffectBase* Play2D(const std::string& typeName, const Transform& trs, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// エフェクトが再生されているか
	/// </summary>
	/// <param name="typeName">エフェクトのデータ名</param>
	/// <param name="label">判別名</param>
	/// <returns>再生されていた場合、その実体を返す。再生されていなければnullptrを返す。</returns>
	EffectBase* IsPlaying(const std::string& typeName, const std::string& label = "");

	//================================================================================
	// ▼停止処理

	/// <summary>
	/// エフェクトを停止する
	/// </summary>
	/// <param name="typeName">エフェクトのデータ名</param>
	/// <param name="label">判別名</param>
	void Stop(const std::string& typeName, const std::string& label);

	/// <summary>
	/// 全てのエフェクトを停止する
	/// </summary>
	void StopAll();

	//================================================================================
	// ▼データ解放処理

	/// <summary>
	/// Csvデータから再度読み込みを行う
	/// </summary>
	void Reload();

	/// <summary>
	/// 全てのエフェクトデータを解放する
	/// </summary>
	void AllReleaseInfo();

#ifdef IMGUI
	
	/// <summary>
	/// デバッグで使用するImGui関連の機能の初期化を行う(読み込んだエフェクトのプレビューを行う)
	/// </summary>
	void InitImGui();

#endif // IMGUI

}