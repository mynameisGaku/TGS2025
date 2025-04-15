#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのTextの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_Text : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのTextを生成する
	/// </summary>
	/// <param name="lable">自身の名前</param>
	/// <param name="format">表示する文字列</param>
	/// <param name="func">ImGuiNode_Baseを引数に持つ関数(文字列を変更したい場合に、関数を別で作ってから、この引数に渡してあげる)</param>
	ImGuiNode_Text(std::string lable, std::string format, std::function<void(ImGuiNode_Base*)> func = nullptr);

	~ImGuiNode_Text();

	//================================================================================
	// ▼各種関数

	void Update() override;

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 表示する文字列を設定する
	/// </summary>
	/// <param name="_format">表示する文字列</param>
	inline void SetText(std::string _format) { format = _format; }

private:
	std::string format;	// 表示する文字列
};

#endif // IMGUI
