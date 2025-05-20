#pragma once

// ◇継承元
#include "framework/gameObject.h"

class Object3D;
class Animator;

/// <summary>
/// アニメーションで使用するコマンドを管理する
/// </summary>
class AnimationEvent : public GameObject {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	AnimationEvent(Animator* animator);
	~AnimationEvent();

	//==========================================================================================
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
	/// コマンド実行処理
	/// </summary>
	void CommandProcess();
	
	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// コマンドを設定する
	/// </summary>
	/// <param name="commandType">コマンドの種類</param>
	/// <param name="commandContents">コマンドの実行内容</param>
	/// <param name="executeAnimFrame">コマンドを実行するアニメーションフレーム</param>
	void SetCommand(std::string commandType, std::string commandContents, float executeAnimFrame);

	inline void SetCommandType(std::string type) { commandType = type; }
	inline void SetCommandContents(std::string contents) { commandContents = contents; }
	inline void SetExecuteAnimFrame(float frame) { executeAnimFrame = frame; }
	inline void SetUseCommand(bool value) { canUseCommande = value; }

	//==========================================================================================
	// ▼ゲッター

	inline std::string CommandType() const { return commandType; }
	inline std::string CommandContents() const { return commandContents; }
	inline float ExecuteAnimFrame() const { return executeAnimFrame; }
	inline bool CanUseCommand() const { return canUseCommande; }

private:
	Object3D* parent;			// 親オブジェクト
	Animator* animator;			// アニメーション再生機構
	std::string commandType;	// コマンドの種類
	std::string commandContents;// コマンドの実行内容
	float executeAnimFrame;		// コマンドを実行するアニメーションフレーム
	bool canUseCommande;		// コマンドの実行権限
};