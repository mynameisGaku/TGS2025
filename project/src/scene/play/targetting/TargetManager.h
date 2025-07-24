#pragma once
#include "framework/GameObject.h"
#include <unordered_map>

class BallManager;
class CharaManager;

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

	int TargetID(int charaIndex);

private:
	/// <summary>
	/// 狙われているボールの位置にマーカーを描画します
	/// </summary>
	/// <param name="ballPos">ボールの座標</param>
	/// <param name="targetCharaID">マーカーを表示する対象キャラクターのID</param>
	void DrawBallPosMarker(const Vector3& ballPos, int targetCharaID);

	/// <summary>
	/// 外周の赤い警告表示
	/// </summary>
	void DrawWarning();

	/// <summary>
	/// 狙われているボールの位置にドゲを描画します
	/// </summary>
	/// <param name="ballPos">ボールの座標</param>
	/// <param name="targetCharaID">マーカーを表示する対象キャラクターのID</param>
	void DrawThorn(const Vector3& ballPos, int targetCharaID);

	BallManager* ballManager;	// ボールの管理者
	CharaManager* charaManager;	// キャラの管理者

	std::unordered_map<int, int> targetList;	// Key: myIndex, Value: targetIndex
	std::vector<bool> checkCamera;	// カメラの数ぶんのチェック済マーク
	int hArrow; // マーカー用画像
};