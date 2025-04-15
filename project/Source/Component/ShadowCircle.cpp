#include "ShadowCircle.h"

// ◇汎用
#include "../../Library/myDxLib.h"
#include "../../Library/resourceLoader.h"

// ◇個別で必要な物
#include "../Object3D.h"
#include "../Stage.h"

namespace {

	static constexpr float SHADOW_HEIGHT = 1000.0f;
	static constexpr float SHADOW_SIZE = 70;
}

ShadowCircle::ShadowCircle() {

	handle = ResourceLoader::LoadGraph("data/texture/shadow/Shadow.tga");
}

ShadowCircle::~ShadowCircle() {

}

void ShadowCircle::Draw() {

	Object3D* obj = dynamic_cast<Object3D*>(parent);

	if (obj == nullptr)
		return;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	float height = SHADOW_HEIGHT * obj->transform->Global().scale.x;
	float size = SHADOW_SIZE * obj->transform->Global().scale.x;

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResDim = MV1CollCheck_Capsule(Stage::Ground()->Model(), -1, obj->transform->Global().position, VAdd(obj->transform->Global().position, VGet(0.0f, -height, 0.0f)), size);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitRes = HitResDim.Dim;
	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitRes->Position[0];
		Vertex[1].pos = HitRes->Position[1];
		Vertex[2].pos = HitRes->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitRes->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;

		if (HitRes->Position[0].y > obj->transform->Global().position.y - height)
			Vertex[0].dif.a = static_cast<BYTE>(128 * (1.0f - fabs(HitRes->Position[0].y - obj->transform->Global().position.y) / height));

		if (HitRes->Position[1].y > obj->transform->Global().position.y - height)
			Vertex[1].dif.a = static_cast <BYTE>(128 * (1.0f - fabs(HitRes->Position[1].y - obj->transform->Global().position.y) / height));

		if (HitRes->Position[2].y > obj->transform->Global().position.y - height)
			Vertex[2].dif.a = static_cast <BYTE>(128 * (1.0f - fabs(HitRes->Position[2].y - obj->transform->Global().position.y) / height));

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitRes->Position[0].x - obj->transform->Global().position.x) / (size * 2.0f) + 0.5f;
		Vertex[0].v = (HitRes->Position[0].z - obj->transform->Global().position.z) / (size * 2.0f) + 0.5f;
		Vertex[1].u = (HitRes->Position[1].x - obj->transform->Global().position.x) / (size * 2.0f) + 0.5f;
		Vertex[1].v = (HitRes->Position[1].z - obj->transform->Global().position.z) / (size * 2.0f) + 0.5f;
		Vertex[2].u = (HitRes->Position[2].x - obj->transform->Global().position.x) / (size * 2.0f) + 0.5f;
		Vertex[2].v = (HitRes->Position[2].z - obj->transform->Global().position.z) / (size * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, handle, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);
}
