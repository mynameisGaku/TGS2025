#pragma once
#include "framework/SceneBase.h"
#include <list>

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene : public SceneBase {
public:
	//==================================================
	// ▼コンストラクタ・デストラクタ

	ResultScene(const std::string& name);
	~ResultScene();

	//==================================================
	// ▼各種関数

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// プレイ前の更新処理
	/// </summary>
	void BeforePlayUpdate() override;

	/// <summary>
	/// プレイ中の更新処理
	/// </summary>
	void InPlayUpdate() override;

	/// <summary>
	/// プレイ後の更新処理
	/// </summary>
	void AfterPlayUpdate() override;
};