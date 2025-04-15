#pragma once
#include "../Library/gameObject.h"
#include <unordered_map>

class SettingBase;

/// <summary>
/// 設定項目を管理する
/// </summary>
class SettingManager : public GameObject {
public:
	//==========================================================================================
	// ▼列挙型

	/// <summary>
	/// 設定項目
	/// </summary>
	enum SettingItem {
		Sensi,	// 感度設定
		Volume,	// 音量設定
		Max,
	};

	/// <summary>
	/// ステート
	/// </summary>
	enum class State {
		None = -1,
		Close,	// 閉じている
		Open,	// 開いている
	};

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	SettingManager();
	~SettingManager();

	//==========================================================================================
	// ▼各種関数

	void Start() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// Csvを読み込む
	/// </summary>
	void LoadCsv();

	/// <summary>
	/// Csvに保存する
	/// </summary>
	void SaveCsv();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 設定画面の表示・非表示を設定する
	/// </summary>
	inline void SetDisplay(bool value) { isDisplay = value; }

	/// <summary>
	/// 設定画面の表示を可能にするか
	/// </summary>
	inline void SetCanDisplay(bool value) { canDisplay = value; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 設定画面を開いているか
	/// </summary>
	inline bool IsDisplay() const { isDisplay; }

private:
	void closeStateUpdate();
	void openStateUpdate();

	std::unordered_map<SettingItem, SettingBase*> setting;	// 設定項目

	State state;			// ステート
	SettingItem curItem;	// 現在選択中の選択項目

	bool canDisplay;	// 設定画面の表示を可能にするか
	bool isDisplay;		// 設定画面の表示・非表示
};
