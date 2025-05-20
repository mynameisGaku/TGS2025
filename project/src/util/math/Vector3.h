#pragma once
#include <DxLib.h>
#include <math.h>

/// <summary>
/// 三軸のベクトル(x,y,z)を扱うクラス
/// </summary>
class Vector3 {
public:

	float x;	// x軸
	float y;	// y軸
	float z;	// z軸

	//================================================================================
	// ▼コンストラクタ

	// x,y,zを0で初期化
	Vector3() : Vector3(0.0f, 0.0f, 0.0f) {}

	// 引数でx,y,zを初期化
	Vector3(float v) : Vector3(v, v, v) {}

	// 引数でx,y,zを初期化
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	};

	//================================================================================
	// ▼変換

	// dxlibのVECTOR構造体からVector3への変換
	// VECTORをVector3に代入しようとすると、このコンストラクタで暗黙的に変換してくれる
	Vector3(const VECTOR& v) : Vector3(v.x, v.y, v.z) {};

	// Vector3からdxlibのVECTOR構造体への変換
	// Vector3にVECTORを代入しようとすると、この演算子で暗黙的に変換してくれる
	inline operator const VECTOR() const { return VGet(x, y, z); }

	// ifの条件やbool変数にVector3を入れられる
	// その場合、ベクトルの大きさが0でなければtrueとして扱う
	inline operator bool() const { return (SquareSize() != 0); }

	// Vector3にVector3を代入する
	void operator=(const Vector3& value);

	// Vector3にVECTORを代入する
	void operator=(const VECTOR& value);

	//================================================================================
	// ▼計算

	// ベクトルの足し算、dxlibのVAddと同等
	// a,bというベクトルがあるとすると、a + b == Vector3(a.x + b.x, a.y + b.y, a.z + b.z)
	const Vector3 operator+(const Vector3& value) const;
	const Vector3 operator+(const VECTOR& value) const;

	// ベクトルの引き算、dxlibのVSubと同等
	// a, bというベクトルがあるとすると、a - b == Vector3(a.x - b.x, a.y - b.y, a.z - b.z)
	const Vector3 operator-(const Vector3& value) const;
	const Vector3 operator-(const VECTOR& value) const;

	// ベクトルの各軸で掛け算
	// a,bというベクトルがあるとすると、a * b == Vector3(a.x * b.x, a.y * b.y, a.z * b.z)
	// ベクトルからxz成分だけ取り出したい時に、vec * Vector3(1, 0, 1)って感じで使用
	const Vector3 operator*(const Vector3& value) const;
	const Vector3 operator*(const VECTOR& value) const;

	// ベクトルの各軸で割り算
	// a,bというベクトルがあるとすると、a / b == Vector3(a.x / b.x, a.y / b.y, a.z / b.z)
	const Vector3 operator/(const Vector3& value) const;
	const Vector3 operator/(const VECTOR& value) const;

	// ベクトルのスケーリング、dxlibのVScaleと同等
	// ベクトルの各軸に同じ値を掛ける。
	// aというベクトル、bという値があるとすると、a * b == Vector3(a.x * b, a.y * b, a.z * b)
	const Vector3 operator*(float value) const;
	const Vector3 operator*(int value) const;

	// ベクトルの各軸に同じ値で割る。
	// aというベクトル、bという値があるとすると、a / b == Vector3(a.x / b, a.y / b, a.z / b)
	const Vector3 operator/(float value) const;
	const Vector3 operator/(int value) const;

	// ベクトルに行列を適応
	// dxlibのVTransformと同等
	const Vector3 operator*(const MATRIX& mat) const;

	//================================================================================
	// ▼イコール付きの計算

	// ベクトルの足し算、dxlibのVAddと同等
	// a,bというベクトルがあるとすると、a + b == Vector3(a.x + b.x, a.y + b.y, a.z + b.z)
	void operator+=(const Vector3& value);
	void operator+=(const VECTOR& value);

	// ベクトルの引き算、dxlibのVSubと同等
	// a, bというベクトルがあるとすると、a - b == Vector3(a.x - b.x, a.y - b.y, a.z - b.z)
	void operator-=(const Vector3& value);
	void operator-=(const VECTOR& value);

	// ベクトルの各軸で掛け算
	// a,bというベクトルがあるとすると、a * b == Vector3(a.x * b.x, a.y * b.y, a.z * b.z)
	// ベクトルからxz成分だけ取り出したい時に、vec * Vector3(1, 0, 1)って感じで使用
	void operator*=(const Vector3& value);
	void operator*=(const VECTOR& value);

	// ベクトルの各軸で割り算
	// a,bというベクトルがあるとすると、a / b == Vector3(a.x / b.x, a.y / b.y, a.z / b.z)
	void operator/=(const Vector3& value);
	void operator/=(const VECTOR& value);

	// ベクトルのスケーリング、dxlibのVScaleと同等
	// ベクトルの各軸に同じ値を掛ける。
	// aというベクトル、bという値があるとすると、a * b == Vector3(a.x * b, a.y * b, a.z * b)
	void operator*=(float value);
	void operator*=(int value);

	// ベクトルの各軸に同じ値で割る。
	// aというベクトル、bという値があるとすると、a / b == Vector3(a.x / b, a.y / b, a.z / b)
	void operator/=(float value);
	void operator/=(int value);

	// ベクトルに行列を適応
	// dxlibのVTransformと同等
	void operator*=(const MATRIX& mat);

	// ベクトルの各軸に同じ値を代入。
	// aというベクトル、bという値があるとすると、a = b == Vector3(a.x = b, a.y = b, a.z = b)
	void operator=(float value);
	void operator=(int value);

	//================================================================================
	// ▼論理

	// 二つのベクトルの値が同じならtrue
	bool operator==(const Vector3& value) const;
	bool operator==(const VECTOR& value) const;

	// 二つのベクトルの値が異なっていればtrue
	bool operator!=(const Vector3& value) const;
	bool operator!=(const VECTOR& value) const;

	//================================================================================
	// ▼各種関数

	//　各要素の合算値を取得する
	inline const float Total() const { return x + y + z; }

	//　各要素の平均値を取得する
	inline const float Average() const { return Total() / 3.0f; }
	
	// ベクトルの長さを取得する時、三平方の定理の、最後の平方根を取る部分を飛ばした値。つまり、長さの2乗の値
	// 長さを比較した結果は、長さの2乗を比較した結果と同じなので、球体当たり判定などで長さの比較があるとき代用可能
	// 平方根は結構重い処理なので、代用可能な場所ではSize()よりSquareSize()を使うのが好ましい
	// dxlibのVSquareSizeと同等
	inline const float SquareSize() const { return x * x + y * y + z * z; }

	// ベクトルの長さを取得
	// dxlibのVSizeと同等
	inline const float Size() const { return sqrtf(SquareSize()); }

	// ベクトルの向きはそのまま、長さを1にしたベクトル(正規化ベクトル)を取得
	// ゼロベクトルを入れた場合ゼロベクトルを返す
	// dxlibのVNormと同等
	inline const Vector3 Norm() const {
		float size = Size();
		if (size == 0.0f)
			return Vector3(0.0f, 0.0f, 0.0f);

		return Vector3(x / size, y / size, z / size);
	}

	// 引数との差を取得する
	inline const Vector3 Distance(Vector3 value) { return Vector3(x - value.x, y - value.y, z - value.z); }

	// 引数から自身の方へ向く角度を取得する
	inline const float Direction(Vector3 value) {
		Vector3 v = Distance(value);
		return atan2f(v.x, v.z);
	}

	// ベクトルのサイズが0以上ならTrueを取得する
	inline const bool IsValue() const { return (SquareSize() != 0.0f); }

	// 垂直関係にある？
	inline const bool IsVertical(const Vector3& r) const {
		float d = VDot(*this, r);
		return (-0.000001f < d && d < 0.000001f);	// 誤差範囲内なら垂直と判定
	}

	// 平行関係にある？
	inline const bool IsParallel(const Vector3& r) const {
		Vector3 cross = VCross(*this, r);
		float d = cross.SquareSize();
		return (-0.000001f < d && d < 0.000001f);	// 誤差範囲内なら平行と判定
	}

	// 鋭角関係？
	inline const bool IsSharpAngle(const Vector3& r) const { return (VDot(*this, r) >= 0.0f); }

	// iの値によって要素を取得(0 -> x, 1 -> y, 2 -> z)
	// ループ処理で使用
	float Get(int i) const;

	// iの値によって要素にvalueをセット(0 -> x, 1 -> y, 2 -> z)
	// ループ処理で使用
	void Set(int i, float value);
	
	/**
	@brief		2 つのベクトル間の線形補間を行います。
	@param[in]	start	: 開始ベクトル (t = 0.0 のときの値)
	@param[in]	end		: 終了ベクトル (t = 1.0 のときの値)
	@param[in]	t		: 加重係数
	@return		補間結果のベクトル
	@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

};

// Vector3を補助する定数、関数
namespace V3 {

	//================================================================================
	// ▼定数

	// ゼロベクトル
	static const Vector3 ZERO = Vector3(0.0f);

	// 全て1のベクトル
	static const Vector3 ONE = Vector3(1.0f);

	// 上を指す単位ベクトル
	static const Vector3 UP = Vector3(0.0f, 1.0f, 0.0f);

	// 下を指す単位ベクトル
	static const Vector3 DOWN = Vector3(0.0f, -1.0f, 0.0f);

	// 左を指す単位ベクトル
	static const Vector3 LEFT = Vector3(-1.0f, 0.0f, 0.0f);

	// 右を指す単位ベクトル
	static const Vector3 RIGHT = Vector3(1.0f, 0.0f, 0.0f);

	// 前方を指す単位ベクトル
	static const Vector3 FORWARD = Vector3(0.0f, 0.0f, 1.0f);

	// 後方を指す単位ベクトル
	static const Vector3 BACK = Vector3(0.0f, 0.0f, -1.0f);

	// X軸とZ軸を指す単位ベクトル
	static const Vector3 HORIZONTAL = Vector3(1.0f, 0.0f, 1.0f);

	//================================================================================
	// ▼各種関数

	// X座標の要素だけ設定する
	// そのほかの要素は0になる
	inline const Vector3 SetX(float _x) { return Vector3(_x, 0.0f, 0.0f); }

	// Y座標の要素だけ設定する
	// そのほかの要素は0になる
	inline const Vector3 SetY(float _y) { return Vector3(0.0f, _y, 0.0f); }

	// Z座標の要素だけ設定する
	// そのほかの要素は0になる
	inline const Vector3 SetZ(float _z) { return Vector3(0.0f, 0.0f, _z); }

	// 二つのベクトルの内積
	inline float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	// 二つのベクトルの外積
	inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {

		Vector3 result;

		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;

		return result;
	}
}