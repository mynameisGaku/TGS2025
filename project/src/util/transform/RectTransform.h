#pragma once

// ◇汎用
#include "src/util/math/Vector2.h"
#include "Anchor.h"
#include <list>

/// <summary>
/// 座標、回転、拡縮を持つクラス
/// </summary>
/// <author> ミッチ </author>
class RectTransform {
public:
	//==========================================================================================
	// ▼メンバ変数

	Vector2 position;	// 座標
	float rotation;		// 回転
	Vector2 scale;		// 拡縮
	Anchor::Preset preset;	// アンカーポイント

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	RectTransform();
	RectTransform(const Vector2& position, const float& rotation = 0.0f, const Vector2& scale = Vector2::Ones, RectTransform* parent = nullptr);
	RectTransform(Anchor::Preset preset, const Vector2& position = Vector2::Zero, const float& rotation = 0.0f, const Vector2& scale = Vector2::Ones, RectTransform* _parent = nullptr);

	~RectTransform();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 親を設定する
	/// </summary>
	/// <param name="_parent">親にするトランスフォーム</param>
	void SetParent(RectTransform* _parent);

	/// <summary>
	/// 指定したグローバルになるように、ローカルを変更する
	/// </summary>
	/// <param name="global">グローバル座標</param>
	void SetGlobal(const RectTransform& global);

	//================================================================================
	// ▼ゲッター

	// 親の影響を含めたグローバル情報を取得(親のXが10、子のXが20だったら子のグローバル座標はX30、みたいな)
	// 親が未設定なら意味なし
	const RectTransform Global() const;

	// 自身の複製品を取得する
	RectTransform* Copy() const;

	//================================================================================
	// ▼計算

	const RectTransform operator+(const RectTransform& trs) const;
	const RectTransform operator-(const RectTransform& trs) const;
	const RectTransform operator*(const float sca) const;
	const RectTransform operator/(const float sca) const;
	const RectTransform operator-() const;
	void operator+=(const RectTransform& trs);
	void operator-=(const RectTransform& trs);
	void operator*=(const float sca);

	//================================================================================
	// ▼デバッグ

	/// <summary>
	/// デバッグ用にアンカーポイントの座標に白色の円を描画する
	/// </summary>
	static void DrawTest();

private:
	RectTransform* parent;	// 親トランスフォームのポインタ
	std::list<RectTransform*> children;	// 子トランスフォームのポインタのリスト

	//================================================================================
	// ▼各種関数

	// 子を追加
	inline void AddChild(RectTransform* child) { children.push_back(child); }

	// 子をリストから消す
	void RemoveChild(RectTransform* child);

	/// <summary>
	/// 親との連携を解除
	/// </summary>
	/// <param name="_parent">親のポインタ</param>
	/// <param name="removeFromChildren">親の子供リストから消す</param>
	void RemoveParent(RectTransform* _parent, bool removeFromChildren = true);
};
