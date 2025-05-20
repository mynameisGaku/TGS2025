#pragma once
// ◇継承元
#include "component.h"

// ◇汎用
#include <list>

// ◇個別で必要な物
#include "collisionDefine.h"

class HittedData;
class Transform;

/// <summary>
/// 当たり判定の基底コンポーネント
/// </summary>
class ColliderBase : public Component {
public:
	//==========================================================================================
	// ▼public変数

	// 自身のトランスフォーム
	// holderのトランスフォームが自動で親になる
	Transform* transform;

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	ColliderBase();
	virtual ~ColliderBase();

	//==========================================================================================
	// ▼各種関数

	void BaseInit(const ColDefine::ColBaseParam& param);

	virtual void UpdateData();

	// 相手のタグが対象に含まれているか
	bool IsTarget(ColDefine::Tag otherTag);

	//==========================================================================================
	// ▼セッター

	// 当たり判定の系統を設定する
	inline void SetTag(ColDefine::Tag value) { tag = value; }
	
	// 当たり判定を行う相手の当たり判定の系統を設定する
	inline void SetTargetTags(const std::list<ColDefine::Tag>& value) { targetTags = value; }
	
	// 当たり判定を行うかを設定する
	inline void SetIsActive(bool value) { isActive = value; }

	// 当たり判定の描画を行うかを設定する
	inline void SetDraw(bool value) { isDraw = value; }

	//==========================================================================================
	// ▼ゲッター
	inline const Transform Global() const { return *global; }

	// 当たり判定の形を取得する
	inline const ColDefine::Shape Shape() const { return shape; }

	//当たり判定の系統を取得する
	inline const ColDefine::Tag Tag() const { return tag; }

	// 当たり判定を行う相手の当たり判定の系統を取得する
	inline const std::list<ColDefine::Tag> TargetTags() const { return targetTags; }

	// 当たり判定を行うかを取得する
	inline const bool IsActive() const { return isActive; }

	// 当たり判定の描画を行うかを取得する
	inline bool IsDraw() const { return isDraw; }

	// 一度しか当たらない当たり判定で、既に当たった相手を保存した物を取得する
	inline HittedData* HittedDataPtr() const { return hittedData; }

protected:
	Transform* global;	// 当たり判定時のトランスフォーム

	ColDefine::Shape shape;	// 当たり判定の形
	ColDefine::Tag tag;		// 当たり判定の系統
	std::list<ColDefine::Tag> targetTags;	// 当たり判定を行う相手の当たり判定。このリストに無いものとは当たり判定を行わない

	bool isActive;	// 当たり判定を行う
	bool isDraw;	// 当たり判定の可視化

	HittedData* hittedData;	// 一度しか当たらない当たり判定で、既に当たった相手を保存する。連続ヒットで大丈夫ならnullptrにする
};

/// <summary>
/// 一度しか当たらない当たり判定で、既に当たった相手を保存する
/// </summary>
class HittedData {
public:
	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 既に当たっているか調べる
	/// </summary>
	/// <param name="other">調べるポインタ</param>
	/// <returns>当たっていたらtrue</returns>
	bool IsHitted(ColliderBase const* other);

	// 当たったリスト追加
	inline void AddHitted(ColliderBase const* hit) { hitted.push_back(hit); }

	// 当たったリストをリセット
	inline void Reset() { hitted.clear(); }

private:
	//==========================================================================================
	// ▼メンバ変数

	// 既に当たり判定を行った相手
	std::list<ColliderBase const*> hitted;
};