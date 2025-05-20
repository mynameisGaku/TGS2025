#pragma once
// ◇継承元
#include "src/util/object3D/Object3D.h"

// ◇汎用
#include <string>

// ◇個別で必要な物
#include "src/common/stage/stageDefine.h"

using namespace StageDefine;

/// <summary>
/// ステージオブジェクトの情報
/// </summary>
class StageObject : public Object3D {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	StageObject(StageObjInfo info, const Transform& _transform, int _id, bool _collider);
	~StageObject();

	void Draw() override;

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// IDを設定する
	/// </summary>
	inline void SetID(int _id) { id = _id; }

	/// <summary>
	/// 引数の値をIDに加算する
	/// </summary>
	inline void AddingID(int value) { id += value; }

	/// <summary>
	/// 当たり判定を機能させるかを設定する
	/// </summary>
	/// <param name="value">Trueの場合、当たり判定を行う</param>
	inline void SetIsCollider(bool value) { isCollider = value; }

	/// <summary>
	/// 当たり判定用モデルの描画を切り替る
	/// </summary>
	/// <param name="value">Trueの場合、描画を行う</param>
	inline void SetIsHitModelDraw(bool value) { isHitModelDraw = value; }
	
	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// IDを取得する
	/// </summary>
	inline int ID() const { return id; }

	/// <summary>
	/// 当たり判定が機能しているかを取得する
	/// </summary>
	inline bool IsCollider() const { return isCollider; }
	inline bool& IsColliderPtr() { return isCollider; }

	/// <summary>
	/// 当たり判定用モデルの描画切り替えを取得する
	/// </summary>
	inline bool IsHitModelDraw() const { return isHitModelDraw; }

	/// <summary>
	/// ステージオブジェクトの情報を取得する
	/// </summary>
	inline StageObjInfo Info() const { return info; }

private:
	StageObjInfo info;	// ステージオブジェクトの情報
	int id;				// ID
	bool isCollider;	// 当たり判定の可否
	bool isHitModelDraw;// 当たり判定用モデルの描画切り替え
};