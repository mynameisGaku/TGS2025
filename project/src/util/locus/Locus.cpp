#include "src/util/locus/Locus.h"
#include <assert.h>
#include "framework/SceneManager.h"

Locus::Locus() {

	hImage = -1;
	SceneManager::CurrentScene()->SetDrawOrder(this, 200);
}

Locus::~Locus() {

	if (hImage > 0) {
		DeleteGraph(hImage);
	}
}

void Locus::Update() {

	for (auto itr = buf.begin(); itr != buf.end(); ) {
		itr->counter++;
		if (itr->counter >= KEEP_NUM) {
			itr = buf.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void Locus::Draw() {

	if (buf.size() < 2)
		return;

	std::vector<VERTEX3D> vertexes;			// 頂点バッファ
	std::vector<unsigned short> indexes;	// インデックスバッファ
	float vdif = 1.0f / (buf.size() - 1);

	for (int i = 0; i < buf.size() - 1; i++) {

		// 補間しながら頂点をつくる
		VECTOR rPos[4]; // rootの補間用の４頂点
		rPos[0] = (i == 0) ? buf[0].root : buf[i - 1].root;
		rPos[1] = buf[i].root;
		rPos[2] = buf[i + 1].root;
		rPos[3] = (i == buf.size() - 2) ? buf[buf.size() - 1].root : buf[i + 2].root;

		VECTOR tPos[4]; // tipの補間用の４頂点
		tPos[0] = (i == 0) ? buf[0].tip : buf[i - 1].tip;
		tPos[1] = buf[i].tip;
		tPos[2] = buf[i + 1].tip;
		tPos[3] = (i == buf.size() - 2) ? buf[buf.size() - 1].tip : buf[i + 2].tip;

		float v0 = (float)buf[i].counter / KEEP_NUM;
		float v1 = (float)buf[i + 1].counter / KEEP_NUM;

		MakeDiv(vertexes, rPos, tPos, v0, v1);
	}

	for (int i = 0; i < (vertexes.size() - 2) / 2; i++) {
		indexes.push_back(i * 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 2);
		indexes.push_back(i * 2 + 1);
		indexes.push_back(i * 2 + 3);
	}

	SetUseZBuffer3D(FALSE);
	SetUseLighting(FALSE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	int indexSize = static_cast<int>(indexes.size());
	int vertexSize = static_cast<int>(vertexes.size());

	if (indexSize >= 3)
		DrawPolygonIndexed3D(&vertexes[0], vertexSize, &indexes[0], indexSize / 3, hImage, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
}

void Locus::SetGraph(std::string filename) {

	hImage = LoadGraph(filename.c_str());
	assert(hImage >= 0);
}

void Locus::SetPosition(VECTOR root, VECTOR tip)
{
	if (buf.size() >= KEEP_NUM) {
		buf.erase(buf.begin());
	}

	POSITION p;
	p.root = root;
	p.tip = tip;
	p.counter = 0;
	buf.push_back(p);
}

void Locus::MakeDiv(std::vector<VERTEX3D>& vs, VECTOR rPos[4], VECTOR tPos[4], float v1, float v2)
{
	const int divNum = 4;

	// root補間の変数を作る
	VECTOR rV0 = (rPos[2] - rPos[0]) / 2.0f;
	VECTOR rV1 = (rPos[3] - rPos[1]) / 2.0f;
	VECTOR rPar1 = 2 * rPos[1] - 2 * rPos[2] + rV0 + rV1;
	VECTOR rPar2 = -3 * rPos[1] + 3 * rPos[2] - 2 * rV0 - rV1;
	
	// tip補間の変数を作る
	VECTOR tV0 = (tPos[2] - tPos[0]) / 2.0f;
	VECTOR tV1 = (tPos[3] - tPos[1]) / 2.0f;
	VECTOR tPar1 = 2 * tPos[1] - 2 * tPos[2] + tV0 + tV1;
	VECTOR tPar2 = -3 * tPos[1] + 3 * tPos[2] - 2 * tV0 - tV1;
	
	VERTEX3D v;
	v.norm = VGet(0, 1, 0);
	v.dif = GetColorU8(255, 255, 255, 255);
	v.spc = GetColorU8(255, 255, 255, 255);

	for (int i = 0; i < divNum; i++) {
		float t = (float)i / divNum;
		float t2 = t * t;
		float t3 = t2 * t;

		// rootの補間をしてpush_back
		v.pos = rPar1 * t3 + rPar2 * t2 + rV0 * t + rPos[1];
		v.u = 0.0f; v.v = (v2 - v1) * t + v1;
		vs.push_back(v);
		
		// tipの補間をしてpush_back
		v.pos = tPar1 * t3 + tPar2 * t2 + tV0 * t + tPos[1];
		v.u = 1.0f; v.v = (v2 - v1) * t + v1;
		vs.push_back(v);
	}
}
