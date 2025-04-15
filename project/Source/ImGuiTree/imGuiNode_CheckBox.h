#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

/// <summary>
/// ImGuiのCheckBoxの機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_CheckBox : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのCheckBoxを生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="value">変動させる値のアドレス</param>
	ImGuiNode_CheckBox(std::string label, bool* value);

	~ImGuiNode_CheckBox();

	//================================================================================
	// ▼各種関数

	void Update() override;

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 変動させる値を設定する
	/// </summary>
	/// <param name="value">変動させる値のアドレス</param>
	inline void SetValue(bool* value) { isCheck = value; }

private:
	bool* isCheck;	// 可否
};

#endif // IMGUI