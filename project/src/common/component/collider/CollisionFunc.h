#pragma once

// ◇汎用
#include "src/util/math/vector2.h"
#include "src/util/math/vector3.h"

// ◇個別で必要な物
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/component/collider/CollisionData.h"
#include "src/common/component/collider/ColliderBase.h"
#include "src/common/component/collider/ColliderSphere.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/common/component/collider/ColliderModel.h"

using namespace ColDefine;

/// <summary>
/// 当たり判定の関数
/// </summary>
namespace ColFunction {

	/// <summary>
	/// ColliderBaseを使用した当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderBaseのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderBaseのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData* ColCheck(ColliderBase* col1, ColliderBase* col2);

	//==========================================================================================
	// ▼球形

	/// <summary>
	/// 3Dの球と球の当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderSphereのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderSphereのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_SphereToSphere(ColliderSphere* col1, ColliderSphere* col2);

	/// <summary>
	/// 3Dの球とカプセルの当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderSphereのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderCapsuleのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_SphereToCapsule(ColliderSphere* col1, ColliderCapsule* col2);

	/// <summary>
	/// 3Dの球とモデルの当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderSphereのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderModelのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_SphereToModel(ColliderSphere* col1, ColliderModel* col2);

	//==========================================================================================
	// ▼カプセル

	/// <summary>
	/// 3Dのカプセルとカプセルの当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderCapsuleのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderCapsuleのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_CapsuleToCapsule(ColliderCapsule* col1, ColliderCapsule* col2);

	/// <summary>
	/// 3Dのカプセルとモデルの当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderCapsuleのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderModelのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_CapsuleToModel(ColliderCapsule* col1, ColliderModel* col2);

	//==========================================================================================
	// ▼モデル

	/// <summary>
	/// 3Dのモデルとモデルの当たり判定
	/// </summary>
	/// <param name="_col1">当たり判定を行うColliderModelのポインタ</param>
	/// <param name="_col2">当たり判定を行うColliderModelのポインタ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck_ModelToModel(ColliderModel* col1, ColliderModel* col2);

	//==========================================================================================
	// ▼コーン

	CollisionData ColCheck_ConeToPoint(const Cone& cone, const Vector3& point);

	//==========================================================================================
	// ▼計算

	/// <summary>
	/// 3Dの当たり判定(円)
	/// </summary>
	/// <param name="p1">当たり判定を行う座標</param>
	/// <param name="r1">p1の判定の大きさ(半径)</param>
	/// <param name="p2">当たり判定を行う座標</param>
	/// <param name="r2">p2の判定の大きさ(半径)</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck3D_Circle(Vector3 p1, float r1, Vector3 p2, float r2);

	/// <summary>
	/// 2Dの当たり判定(円)
	/// </summary>
	/// <param name="p1">当たり判定を行う座標</param>
	/// <param name="r1">p1の判定の大きさ(半径)</param>
	/// <param name="p2">当たり判定を行う座標</param>
	/// <param name="r2">p2の判定の大きさ(半径)</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck2D_Circle(Vector2 p1, float r1, Vector2 p2, float r2);

	/// <summary>
	/// 2Dの当たり判定(四角・始点終点)
	/// </summary>
	/// <param name="p1">当たり判定を行う座標</param>
	/// <param name="s1">p1の判定の大きさ(右下)</param>
	/// <param name="p2">当たり判定を行う座標</param>
	/// <param name="s2">p2の判定の大きさ(右下)</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck2D_Box(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2);

	/// <summary>
	/// 2Dの当たり判定(四角・ベクトル)
	/// </summary>
	/// <param name="p1">当たり判定を行う座標</param>
	/// <param name="p2">当たり判定を行う座標</param>
	/// <param name="v">判定をとるベクトルの大きさ</param>
	/// <returns>当たった場合:True</returns>
	CollisionData ColCheck2D_Box(Vector2 p1, Vector2 p2, Vector2 v);

	/// <summary>
	/// ∠p1p2p3が鋭角を取得する
	/// </summary>
	/// <param name="p1">座標1</param>
	/// <param name="p2">座標2</param>
	/// <param name="p3">座標3</param>
	/// <returns>鋭角の場合、true</returns>
	bool IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	/// <summary>
	/// 点と直線の最短距離を取得する
	/// </summary>
	/// <param name="p">点</param>
	/// <param name="l">直線</param>
	/// <param name="h">点から下ろした垂線の足(戻り値)</param>
	/// <param name="t">ベクトル係数(戻り値)</param>
	/// <returns>最短距離</returns>
	float CalcPointLineDist(const Vector3& p, const ColDefine::Lay& l, Vector3& h, float& t);

	/// <summary>
	/// 点と線分の最短距離
	/// </summary>
	/// <param name="p">点</param>
	/// <param name="seg">線分</param>
	/// <param name="h">最短距離となる端点(戻り値)</param>
	/// <param name="t">端点位置( t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 )</param>
	/// <returns>最短距離</returns>
	float CalcPointSegmentDist(const Vector3& p, const ColDefine::Segment& seg, Vector3& h, float& t);

	/// <summary>
	/// 2直線の最短距離
	/// </summary>
	/// <param name="l1">直線1</param>
	/// <param name="l2">直線2</param>
	/// <param name="p1">直線1の垂線の足(戻り値)</param>
	/// <param name="p2">直線2の垂線の足(戻り値)</param>
	/// <param name="t1">直線1のベクトル係数(戻り値)</param>
	/// <param name="t2">直線1のベクトル係数(戻り値)</param>
	/// <returns>最短距離</returns>
	float CalcLineLineDist(const ColDefine::Lay& l1, const ColDefine::Lay& l2, Vector3& p1, Vector3& p2, float& t1, float& t2);

	/// <summary>
	/// 2線分の最短距離
	/// </summary>
	/// <param name="s1">線分1</param>
	/// <param name="s2">線分2</param>
	/// <param name="p1">線分1の垂線の足(戻り値)</param>
	/// <param name="p2">線分2の垂線の足(戻り値)</param>
	/// <param name="t1">線分1のベクトル係数(戻り値)</param>
	/// <param name="t2">線分2のベクトル係数(戻り値)</param>
	/// <returns>最短距離</returns>
	float CalcSegmentSegmentDist(const ColDefine::Segment& s1, const ColDefine::Segment& s2, Vector3& p1, Vector3& p2, float& t1, float& t2);

	float CollCheck_PointToAABB(const Vector3& point, const ColDefine::AABB& aabb);
};