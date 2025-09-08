#pragma once
#include "framework/GameObject.h"
#include <unordered_map>

class BallManager;
class CharaManager;

class UI_Target;

/// <summary>
/// ロックオン時のUI、被ロックオン時のUIを描画する
/// </summary>
/// <author>ミッチ</author>
class TargetManager : public GameObject {
public:
	TargetManager();
	~TargetManager();

	void Start() override;
	void Update() override;
	void Draw() override;

	void UpdateUI();
	void DrawUI();

private:
	/// <summary>
	/// 狙われているボールの位置にドゲを描画します
	/// </summary>
	/// <param name="ballPos">ボールの座標</param>
	/// <param name="targetCharaID">マーカーを表示する対象キャラクターのID</param>
	void DrawThorn();

	BallManager* ballManager;	// ボールの管理者
	CharaManager* charaManager;	// キャラの管理者

	UI_Target* m_UI_Target;
};