#pragma once
// ◇継承元
#include "../Library/sceneBase.h"

// ◇汎用
#include <list>

// ◇UI関連
class UI_Canvas;

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public SceneBase {
public:
	//==================================================
	// ▼コンストラクタ・デストラクタ

	TitleScene(std::string name);
	~TitleScene();

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