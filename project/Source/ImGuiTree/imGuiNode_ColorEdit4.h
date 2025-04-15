#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

#ifdef IMGUI

// ◇個別で必要な物
#include <vector>

/// <summary>
/// ImGuiのColorEdit4の機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiNode_ColorEdit4 : public ImGuiNode_Base {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ImGuiのColorEdit4を生成する
	/// </summary>
	/// <param name="label">自身の名前</param>
	/// <param name="rgba">R,G,B,Aの数値のアドレス</param>
	ImGuiNode_ColorEdit4(std::string label, float* rgba[4]);

	~ImGuiNode_ColorEdit4();

	//================================================================================
	// ▼各種関数

	void Update() override;

private:
	float* rgba[4];	// R,G,B,Aの数値
};

#endif // IMGUI