#include "CollisionFunc.h"
#include "CollisionData.h"

// ◇汎用
#include "../Util/Utils.h"

using namespace ColDefine;

CollisionData* ColFunction::ColCheck(ColliderBase* col1, ColliderBase* col2) {

	// IDの小さいやつを左に据える
	const int col1ID = static_cast<int>(col1->Shape());
	const int col2ID = static_cast<int>(col2->Shape());

	if (col1ID > col2ID)
		std::swap<ColliderBase*>(col1, col2);

	switch (col1->Shape()) {
	case Shape::sSphere:
		switch (col2->Shape()) {
			case Shape::sSphere:	return new CollisionData(ColCheck_SphereToSphere(dynamic_cast<ColliderSphere*>(col1),	dynamic_cast<ColliderSphere*>(col2)));	break;
			case Shape::sCapsule:	return new CollisionData(ColCheck_SphereToCapsule(dynamic_cast<ColliderSphere*>(col1),	dynamic_cast<ColliderCapsule*>(col2)));	break;
			case Shape::sModel:		return new CollisionData(ColCheck_SphereToModel(dynamic_cast<ColliderSphere*>(col1),	dynamic_cast<ColliderModel*>(col2)));	break;
			default:				return nullptr;
		}
		break;

	case Shape::sCapsule:
		switch (col2->Shape()) {
		case Shape::sSphere:	return new CollisionData(ColCheck_SphereToCapsule(dynamic_cast<ColliderSphere*>(col2),		dynamic_cast<ColliderCapsule*>(col1)));	break;
		case Shape::sCapsule:	return new CollisionData(ColCheck_CapsuleToCapsule(dynamic_cast<ColliderCapsule*>(col1),	dynamic_cast<ColliderCapsule*>(col2)));	break;
		default:				return nullptr;
		}
		break;

	default:
		return nullptr;
	}

	return nullptr;
}

CollisionData ColFunction::ColCheck_SphereToSphere(ColliderSphere* col1, ColliderSphere* col2) {

	const Transform col1GlobalTransform = col1->transform->Global();	// col1の絶対座標
	const Transform col2GlobalTransform = col2->transform->Global();	// col2の絶対座標

	const float squareDistance = (col2GlobalTransform.position - col1GlobalTransform.position).SquareSize();	// col1とcol2の差の長さ

	const float sumRadius = col1->Radius() + col2->Radius();	// 当たり判定の範囲

	bool isCollision = sumRadius * sumRadius > squareDistance;

	return CollisionData(isCollision);
}

CollisionData ColFunction::ColCheck_SphereToCapsule(ColliderSphere* col1, ColliderCapsule* col2) {

	const Transform col1_GlobalTrs = col1->transform->Global();	// col1の絶対座標
	const Transform col2_GlobalTrs = col2->transform->Global();	// col2の絶対座標

	Vector3 vecNear = V3::ZERO;		// 点と線分の最近傍点

	Vector3 begin = col2_GlobalTrs.position;// カプセルコライダーの始点
	Vector3 end = begin + col2->OffsetWorld();	// カプセルコライダーの終点
	Vector3 point = col1_GlobalTrs.position;// 球形コライダーの座標

	Vector3 vecLine = (end - begin).Norm();	// カプセルコライダーの始点と終点の線分の方向を表すベクトル
	Vector3 vecFaceBeing = point - begin;	// 始点から点に向かうベクトル
	Vector3 vecFaceEnd = point - end;		// 終点から点に向かうベクトル

	// 線分の左側に居るか
	if (V3::Dot(vecLine, vecFaceBeing) < 0)
		vecNear = begin;
	// 線分の右側に居るか
	else if (V3::Dot(vecLine, vecFaceEnd) > 0)
		vecNear = end;
	// 線分の内側に居るか
	else
		vecNear = vecLine * vecFaceBeing.Size();

	Vector3 distance = vecNear - point;	// 点から最近傍点との距離

	const float squareDistance = distance.SquareSize();		// 差の長さ
	const float sumRadius = col1->Radius() + col2->Radius();// 当たり判定の範囲

	bool isCollision = (sumRadius * sumRadius > squareDistance);

	return CollisionData(isCollision);
}

CollisionData ColFunction::ColCheck_CapsuleToCapsule(ColliderCapsule* col1, ColliderCapsule* col2) {

	using namespace ColDefine;

	const Transform col1_GlobalTrs = col1->transform->Global();	// col1の絶対座標
	const Transform col2_GlobalTrs = col2->transform->Global();	// col2の絶対座標

	Segment s1, s2;
	Vector3 p1, p2;
	float	t1, t2;

	s1.point = col1_GlobalTrs.position;
	s1.vec	 = col1->OffsetLocal();

	s2.point = col2_GlobalTrs.position;
	s2.vec	 = col2->OffsetLocal();

	float d = CalcSegmentSegmentDist(s1, s2, p1, p2, t1, t2);

	bool isCollsion = (d <= col1->Radius() + col2->Radius());

	return CollisionData(isCollsion);
}

CollisionData ColFunction::ColCheck_SphereToModel(ColliderSphere* col1, ColliderModel* col2) {

	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(col2->Model(), -1, col1->transform->Global().position, col1->Radius());

	if (result.HitNum == 0)
	{
		return CollisionData(false);
	}
	else
	{
		std::list<CollisionData::HitPolyData> datas;
		for (int i = 0; i < result.HitNum; i++)
		{
			CollisionData::HitPolyData data;
			data.collisionPoint = result.Dim[i].HitPosition;
			data.normal = result.Dim[i].Normal;

			datas.push_back(data);
		}

		return CollisionData(true, nullptr, nullptr, datas);
	}
}

const CollisionData& ColFunction::ColCheck_ConeToPoint(const Cone& cone, const Vector3& point) {

	// コーンの位置からポイントまでの距離
	const Vector3 dist = point - cone.transform.position;

	// 範囲外の場合
	if (dist.Size() >= cone.range)
		return CollisionData(false);

	// コーンの向く方向
	const Vector3 forward = V3::FORWARD * cone.transform.RotationMatrix();

	// 内積外の場合
	if (VDot(VNorm(dist), forward) < cosf(Math::DegToRad(cone.angle)))
		return CollisionData(false);
	
	return CollisionData(true);
}

CollisionData ColFunction::ColCheck3D_Circle(Vector3 p1, float r1, Vector3 p2, float r2) {

	bool isCollision = false;

	float distance = (p1 - p2).Size();
	float r = r1 + r2;

	if (distance < r)
		isCollision = true;

	return CollisionData(isCollision);
}

CollisionData ColFunction::ColCheck2D_Circle(Vector2 p1, float r1, Vector2 p2, float r2) {

	bool isCollision = false;

	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;

	if (dx * dx + dy * dy < (r1 + r2) * (r1 + r2))
		isCollision = true;

	return CollisionData(isCollision);
}

CollisionData ColFunction::ColCheck2D_Box(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2) {

	bool isCollision = false;

	if (p1.x < p2.x + s2.x &&
		p1.x + s1.x > p2.x &&
		p1.y < p2.y + s2.y &&
		p1.y + s1.y > p2.y)
		isCollision = true;

	return CollisionData(isCollision);
}

CollisionData ColFunction::ColCheck2D_Box(Vector2 p1, Vector2 p2, Vector2 v) {

	float vx = p2.x - p1.x;
	float vy = p2.y - p1.y;

	if (vx < 0) vx *= -1;
	if (vy < 0) vy *= -1;

	bool isCollision = (vx < v.x && vy < v.y);

	return CollisionData(isCollision);
}

bool ColFunction::IsSharpAngle(const Vector3& p1, const Vector3& p2, const Vector3& p3) {

	return (p1 - p2).IsSharpAngle(p3 - p2);
}

float ColFunction::CalcPointLineDist(const Vector3& p, const ColDefine::Lay& l, Vector3& h, float& t) {

	float lenSqV = l.vec.SquareSize();
	t = 0.0f;
	if (lenSqV > 0.0f)
		t = V3::Dot(l.vec, p - l.point) / lenSqV;

	h = l.point + l.vec * t;
	return (h - p).Size();
}

float ColFunction::CalcPointSegmentDist(const Vector3& p, const ColDefine::Segment& seg, Vector3& h, float& t) {

	const Vector3 e = seg.getEndPoint();

	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = CalcPointLineDist(p, Lay(seg.point, e - seg.point), h, t);

	if (IsSharpAngle(p, seg.point, e) == false) {
		// 始点側の外側
		h = seg.point;
		return (seg.point - p).Size();
	}
	else if (IsSharpAngle(p, e, seg.point) == false) {
		// 終点側の外側
		h = e;
		return (e - p).Size();
	}

	return len;
}

float ColFunction::CalcLineLineDist(const ColDefine::Lay& l1, const ColDefine::Lay& l2, Vector3& p1, Vector3& p2, float& t1, float& t2) {

	// 2直線が平行？
	if (l1.vec.IsParallel(l2.vec) == true) {

		// 点P11と直線L2の最短距離の問題に帰着
		float len = CalcPointLineDist(l1.point, l2, p2, t2);
		p1 = l1.point;
		t1 = 0.0f;

		return len;
	}

	// 2直線はねじれ関係
	float DV1V2 = V3::Dot(l1.vec, l2.vec);
	float DV1V1 = l1.vec.SquareSize();
	float DV2V2 = l2.vec.SquareSize();
	Vector3 P21P11 = l1.point - l2.point;

	t1 = (DV1V2 * V3::Dot(l2.vec, P21P11) - DV2V2 * V3::Dot(l1.vec, P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = l1.getPoint(t1);
	t2 = V3::Dot(l2.vec, (p1 - l2.point)) / DV2V2;
	p2 = l2.getPoint(t2);

	return (p2 - p1).Size();
}

float ColFunction::CalcSegmentSegmentDist(const ColDefine::Segment& s1, const ColDefine::Segment& s2, Vector3& p1, Vector3& p2, float& t1, float& t2) {

	// S1が縮退している？
	if (s1.vec.SquareSize() < _OX_EPSILON_) {
		// S2も縮退？
		if (s2.vec.SquareSize() < _OX_EPSILON_) {
			// 点と点の距離の問題に帰着
			float len = (s2.point - s1.point).Size();
			p1 = s1.point;
			p2 = s2.point;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1の始点とS2の最短問題に帰着
			float len = CalcPointSegmentDist(s1.point, s2, p2, t2);
			p1 = s1.point;
			t1 = 0.0f;
			t2 = Math::Clamp(t2, 0.0f, 1.0f);
			return len;
		}
	}

	// S2が縮退している？
	else if (s2.vec.SquareSize() < _OX_EPSILON_) {
		// S2の始点とS1の最短問題に帰着
		float len = CalcPointSegmentDist(s2.point, s1, p1, t1);
		p2 = s2.point;
		t1 = Math::Clamp(t1, 0.0f, 1.0f);
		t2 = 0.0f;
		return len;
	}

	/* 線分同士 */

	// 2線分が平行だったら垂線の端点の一つをP1に仮決定
	if (s1.vec.IsParallel(s2.vec) == true) {
		t1 = 0.0f;
		p1 = s1.point;
		float len = CalcPointSegmentDist(s1.point, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// 線分はねじれの関係
		// 2直線間の最短距離を求めて仮のt1,t2を求める
		float len = CalcLineLineDist(s1, s2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// 垂線の足が外にある事が判明
	// S1側のt1を0～1の間にクランプして垂線を降ろす
	t1 = Math::Clamp(t1, 0.0f, 1.0f);
	p1 = s1.getPoint(t1);
	float len = CalcPointSegmentDist(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
	t2 = Math::Clamp(t2, 0.0f, 1.0f);
	p2 = s2.getPoint(t2);
	len = CalcPointSegmentDist(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// 双方の端点が最短と判明
	t1 = Math::Clamp(t1, 0.0f, 1.0f);
	p1 = s1.getPoint(t1);
	return (p2 - p1).Size();
}
