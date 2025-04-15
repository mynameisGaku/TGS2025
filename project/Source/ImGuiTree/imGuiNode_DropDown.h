#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

// ◇個別で必要な物
#include <vector>

/// <summary>
/// ImGuiのDropDownの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_DropDown : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのDropDownを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="list">選択肢(string型)</param>
	/// <param name="func">ボタンを押したときの処理</param>
	/// <param name="buttonText">選択するボタンに表示する文字列</param>
	ImGuiNode_DropDown(std::string label, const std::vector<std::string>& list, std::function<void(const std::string&)> func = nullptr, std::string buttonText = "Apply");

	~ImGuiNode_DropDown();

	//================================================================================
	// ▼各種関数

	void Update() override;

	//================================================================================
	// ▼セッター

	/// <summary>
	/// リストを設定する
	/// </summary>
	inline void SetList(const std::vector<std::string>& _list) { list = _list; }

	/// <summary>
	/// ボタンに表示する文字列を設定する
	/// </summary>
	inline void SetButtonText(const std::string& _buttonText) { buttonText = _buttonText; }

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ボタンテキストを取得する
	/// </summary>
	inline std::string ButtonText() const { return buttonText; }

	/// <summary>
	/// 現在選んでいる選択肢を取得する
	/// </summary>
	inline std::string CurrentItem() const { return currentItem; }

	/// <summary>
	/// 現在選んでいる要素数を取得する
	/// </summary>
	inline int CurrentIndex() const { return currentIndex; }

private:
	std::vector<std::string> list;	// 選択肢
	std::string buttonText;			// ボタンテキスト
	std::string currentItem;		// 現在選んでいる選択肢
	int currentIndex;				// 現在選んでいる要素数

	std::function<void(const std::string&)> action;	// ボタンを押したときの処理
};

#endif // IMGUI