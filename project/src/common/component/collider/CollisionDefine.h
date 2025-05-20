#pragma once

// ◇汎用
#include "src/util/transform/Transform.h"
#include <list>

class ColliderBase;

/// <summary>
/// 当たり判定に関する定義値をまとめるnamespace
/// </summary>
namespace ColDefine {

	//================================================================================
	// ▼列挙型

	// 当たり判定の形
	enum class Shape {
		sNone = -1,
		sSphere,	// 球
		sCapsule,	// カプセル
		sModel,		// モデル
	};

	// 当たり判定の系統
	enum class Tag {
		tNone = -1,
		tCharaRed,	// 赤チームキャラ
		tCharaBlue,	// 青チームキャラ
		tBallRed,	// 赤チームボール
		tBallBlue,	// 青チームボール
		tCatchRed,	// 赤チームキャッチ
		tCatchBlue,	// 青チームキャッチ
		tTerrain,	// 地形
		tMax,
	};

	struct ColBaseParam {
		Transform trs;
		Tag tag = Tag::tNone;
		std::list<ColDefine::Tag> targetTags;
		bool onlyOnce = false;
		bool push = false;
	};

	// 点と方向ベクトル
	struct Lay {
		Vector3 point;	// 座標
		Vector3 vec;	// 方向ベクトル
		Lay() : point(Vector3::Zero), vec(Vector3::Zero) {}
		Lay(const Vector3& p, const Vector3& v) : point(p), vec(v) {}
		~Lay() {}

		// 点上の座標を取得
		//  ベクトルに掛け算する係数
		inline Vector3 getPoint(float t) const { return point + vec * t; }
	};

	// 線分
	struct Segment : public Lay {
		Segment() : Lay(Vector3::Zero, Vector3::Zero) {}
		Segment(const Vector3& p, const Vector3& v) : Lay(p, v) {}

		// 終点を取得
		inline Vector3 getEndPoint() const { return point + vec; }
	};

	// 球
	struct Sphere {
		Vector3 point;	// 座標
		float raduis;	// 半径
	};

	// カプセル
	struct Capsule {
		Vector3 begin;	// 線分の始点
		Vector3 end;	// 線分の終点
		float raduis;	// 半径
	};

	// コーン
	struct Cone {
		Transform transform;// 座標・回転・拡縮
		float range;		// 長さ
		float angle;		// 角度
	};

	// AABB
	struct AABB {
		Vector3 p;	// 中心点
		Vector3 hl;	// 各軸の辺の長さの半分
		AABB() {}
		AABB(const Vector3& p, const Vector3& hl) : p(p), hl(hl) {}

		// 辺の長さを取得
		inline float lenX() const { return hl.x * 2.0f; };
		inline float lenY() const { return hl.y * 2.0f; };
		inline float lenZ() const { return hl.z * 2.0f; };

		inline float len(int i) { return *((&hl.x) + i) * 2.0f; }
	};
}