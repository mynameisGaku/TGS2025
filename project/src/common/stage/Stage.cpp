#include "stage.h"

// ◇汎用
#include "src/util/Utils.h"

// ◇演出・機能
//#include "effectManager.h"
//#include "effectBase.h"
//#include "collisionFunc.h"

// ◇個別で必要な物
//#include "charaManager.h"
//#include "player.h"

using namespace StageDefine;

namespace {

	Object3D* ground;	// 地面の情報
	Object3D* sky;		// 空の情報
	Object3D* wall;		// 壁の情報
}

void Stage::Init() {

	if (ground == nullptr)
		ground = new Object3D(MV1LoadModel("data/model/Stage/Ground_000.mv1"), Transform(Vector3::Zero, Vector3::Zero, Vector3(20.0f)));

	if (sky == nullptr)
		sky = new Object3D(MV1LoadModel("data/model/Stage/Sky_000.mv1"), Transform(Vector3::Zero, Vector3::Zero, Vector3(1.0f)));

	if (wall == nullptr)
		wall = new Object3D(MV1LoadModel("data/model/Stage/StageCollider.mv1"), Transform(Vector3::Zero, Vector3::Zero, Vector3(STAGE_RANGE / 50.0f)));

	MV1SetupCollInfo(ground->Model(), -1, 8, 8, 8);	//コリジョン情報を構築する
	MV1RefreshCollInfo(ground->Model(), -1);		//コリジョン情報を更新する

}

void Stage::Update() {

}

void Stage::Draw() {

	if (sky != nullptr)
		sky->Draw();

	SetFogColor(200, 200, 150);
	SetFogStartEnd(150.0f, STAGE_RANGE * 5.0f);
	SetFogEnable(true);

	if (ground != nullptr)
		ground->Draw();

	if (wall != nullptr)
		wall->Draw();

	SetFogEnable(false);
}

void Stage::Release() {

	Function::DeletePointer(ground);
	Function::DeletePointer(sky);
	Function::DeletePointer(wall);
}

bool Stage::ColCheckGround(Vector3 begin, Vector3 end, Vector3* hitPos) {

	MV1_COLL_RESULT_POLY hit;
	hit = MV1CollCheck_Line(ground->Model(), 0, begin, end);
	if (hit.HitFlag > 0) {
		if (hitPos != nullptr)	// 引数にポインタが入っていたら、代入をする
			*hitPos = hit.HitPosition;
		return true;
	}

	return false;
}

bool Stage::ColCheckWall(Vector3 begin, Vector3 end, float r, Vector3* push) {

	bool hitFlag = false;
	VECTOR pushVec = VGet(0, 0, 0);

	MV1SetupCollInfo(wall->Model(), -1, 8, 8, 8);
	MV1_COLL_RESULT_POLY_DIM dim = MV1CollCheck_Capsule(wall->Model(), -1, begin, end, r);
	for (int i = 0; i < dim.HitNum; i++) {
		hitFlag = true;
		// 当たり判定をしてpushVecを作る
		// ポリゴンと線分の距離を求める
		float len = Segment_Triangle_MinLength(begin, end, dim.Dim[i].Position[0], dim.Dim[i].Position[1], dim.Dim[i].Position[2]);
		if (len > r)
			continue;
		// 押し返す量は、(r - len)になる
		// ポリゴンの法線から押し返す量を乗算する
		VECTOR newPush = dim.Dim[i].Normal * (r - len);
		pushVec += newPush;
		VECTOR pushVecNorm = VNorm(pushVec);
		float pushIn = VDot(pushVecNorm, newPush);
		// これが新しく押す量
		if (pushIn < VSize(pushVec)) {
			pushVec += newPush - pushIn * pushVecNorm;
		}
		else {
			pushVec = newPush;
		}
	}
	MV1CollResultPolyDimTerminate(dim);

	if (push != nullptr)
		*push = pushVec * -1.0f;
	return hitFlag;
}

Object3D* Stage::Ground() { return ground; }
Object3D* Stage::Sky() { return sky; }
Object3D* Stage::Wall() { return wall; }
