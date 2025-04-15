#pragma once
#include "../../Source/config.h"

#ifdef IMGUI

// ◇個別で必要な物
#include <string>
#include <functional>

// ◇ImGuiに必要なファイルのインクルード
#include "../../vendor/ImGui/imgui.h"
#include "../../vendor/ImGui/imgui_impl_dx11.h"
#include "../../vendor/ImGui/imgui_impl_win32.h"

/// <summary>
/// ImGuiの機能用の基底クラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_Base {
public:
	//==================================================
	// ▼コンストラクタ・デストラクタ

	ImGuiNode_Base() {};
	virtual ~ImGuiNode_Base() {};

	//==================================================
	// ▼各種関数

	inline virtual void Update() {

		if (func != nullptr)
			func(this);
	}

	//==================================================
	// ▼セッター

	/// <summary>
	/// ImGuiNode内で実行したい関数を設定する(毎フレーム呼び出される)
	/// </summary>
	/// <param name="_func">ImGuiNode内で処理したい関数</param>
	inline void SetFunction(std::function<void(ImGuiNode_Base*)> _func) {

		std::function<void(ImGuiNode_Base*)> fc = nullptr;
		if (_func != nullptr)
			fc = std::bind(_func, this);

		func = fc;
	}

	//==================================================
	// ▼ゲッター

	// 自身の名前を取得する
	inline std::string Label() const { return label; }

protected:
	std::string label;	// 自身の名前
	std::function<void(ImGuiNode_Base*)> func;	// 実行処理
};

#endif // IMGUI
