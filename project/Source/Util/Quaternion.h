#pragma once
#include "DxLib.h"
#include <math.h>

class Quaternion {
public:

	float x;   // x軸
	float y;   // y軸
	float z;   // z軸
	float w;   // 任意軸

	float yaw;
	float pitch;
	float roll;

	VECTOR xAxis, yAxis, zAxis;

	MATRIX matTrans;
	MATRIX matRot;
	MATRIX matScale;
	MATRIX mat;

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	Quaternion();
	Quaternion(float x, float y, float z, float t);
	Quaternion(VECTOR vec, float t);

	~Quaternion();

	//================================================================================
	// ▼変換

	// dxlibのVECTOR構造体からQuaternionへの変換
	// VECTORをQuaternionに代入しようとすると、このコンストラクタで暗黙的に変換してくれる
	inline Quaternion(const VECTOR& v) : Quaternion(v.x, v.y, v.z, 0.0f) {};

	// Vector3からdxlibのVECTOR構造体への変換
	// Vector3にVECTORを代入しようとすると、この演算子で暗黙的に変換してくれる
	operator const VECTOR();

	// ifの条件やbool変数にVector3を入れられる
	// その場合、ベクトルの大きさが0でなければtrueとして扱う
	operator bool() const;

	// クォータニオンにVECTORを代入する
	void operator=(const VECTOR& value);

	//================================================================================
	// ▼計算

	// ベクトルの足し算
	Quaternion  operator+(const VECTOR& value);

	// ベクトルの引き算
	Quaternion operator-(const VECTOR& value);

	// ベクトルの掛け算
	Quaternion operator*(const VECTOR& value);

	// ベクトルの割り算
	Quaternion operator/(const VECTOR& value);

	// ベクトルの足し算
	void operator+=(const VECTOR& value);

	// ベクトルの引き算
	void operator-=(const VECTOR& value);

	// ベクトルの掛け算
	void operator*=(const VECTOR& value);

	// ベクトルの割り算
	void operator/=(const VECTOR& value);

	//クォータニオン同士の掛け算
	Quaternion operator*(const Quaternion& q);
	void operator*=(const Quaternion& q);

	//クォータニオンから回転行列へ
	MATRIX ToMatrix();
};
