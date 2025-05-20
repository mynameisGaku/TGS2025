#pragma once
#include "src/common/component/collider/ColliderBase.h"

/// <summary>
/// モデルの形そのまま当たり判定
/// </summary>
class ColliderModel : public ColliderBase {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	// 初期化
	ColliderModel();
	~ColliderModel() {};

	//==========================================================================================
	// ▼各種関数

	void UpdateData() override;

	//==========================================================================================
	// ▼セッター

	inline void SetRefresh(bool _refresh) { refresh = _refresh; }

	//==========================================================================================
	// ▼ゲッター

	inline int Model() const { return hModel; }
	inline bool Refresh() const { return refresh; }

private:
	//==========================================================================================
	// ▼メンバ変数

	int hModel;		// モデルの情報
	bool refresh;	// 位置情報を更新するか
};
