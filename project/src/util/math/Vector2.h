#pragma once
#include <DxLib.h>
#include <math.h>

/// <summary>
/// 二軸のベクトル(x,y)を扱うクラス
/// </summary>
class Vector2 {
public:

	float x;	// x軸
	float y;	// y軸

	//================================================================================
	// ▼コンストラクタ

	// x,yを0で初期化
	Vector2() : Vector2(0.0f, 0.0f) {}

	// 一つの引数でx,yを初期化
	Vector2(float v) : Vector2(v, v) {}

	// 引数でx,yを初期化
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	};

	//================================================================================
	// ▼変換

	// dxlibのVECTOR構造体からVector2への変換
	// VECTORをVector2に代入しようとすると、このコンストラクタで暗黙的に変換してくれる
	inline Vector2(const VECTOR& v) : Vector2(v.x, v.y) {};

	// Vector2からdxlibのVECTOR構造体への変換
	// Vector2にVECTORを代入しようとすると、この演算子で暗黙的に変換してくれる
	inline operator const VECTOR() const { return VGet(x, y, 0); }

	// ifの条件やbool変数にVector2を入れられる
	// その場合、ベクトルの大きさが0でなければtrueとして扱う
	inline operator bool() const { return (SquareSize() != 0); }

	// Vector2にVector2を代入する
	void operator=(const Vector2& value);

	// Vector2にVECTORを代入する
	void operator=(const VECTOR& value);

	//================================================================================
	// ▼計算

	// ベクトルの足し算、dxlibのVAddと同等
	// a,bというベクトルがあるとすると、a + b == Vector2(a.x + b.x, a.y + b.y)
	const Vector2 operator+(const Vector2& value) const;

	// ベクトルの引き算、dxlibのVSubと同等
	// a, bというベクトルがあるとすると、a - b == Vector2(a.x - b.x, a.y - b.y)
	const Vector2 operator-(const Vector2& value) const;

	// ベクトルの各軸で掛け算
	// a,bというベクトルがあるとすると、a * b == Vector2(a.x * b.x, a.y * b.y)
	const Vector2 operator*(const Vector2& value) const;

	// ベクトルの各軸で割り算
	// a,bというベクトルがあるとすると、a / b == Vector2(a.x / b.x, a.y / b.y)
	const Vector2 operator/(const Vector2& value) const;

	// ベクトルのスケーリング、dxlibのVScaleと同等
	// ベクトルの各軸に同じ値を掛ける。
	// aというベクトル、bという値があるとすると、a * b == Vector2(a.x * b, a.y * b)
	const Vector2 operator*(float value) const;
	const Vector2 operator*(int value) const;

	// ベクトルの各軸に同じ値で割る。
	// aというベクトル、bという値があるとすると、a / b == Vector2(a.x / b, a.y / b)
	const Vector2 operator/(float value) const;
	const Vector2 operator/(int value) const;

	//================================================================================
	// ▼イコール付きの計算

	// ベクトルの足し算、dxlibのVAddと同等
	// a,bというベクトルがあるとすると、a + b == Vector2(a.x + b.x, a.y + b.y)
	void operator+=(const Vector2& value);

	// ベクトルの引き算、dxlibのVSubと同等
	// a, bというベクトルがあるとすると、a - b == Vector2(a.x - b.x, a.y - b.y)
	void operator-=(const Vector2& value);

	// ベクトルの各軸で掛け算
	// a,bというベクトルがあるとすると、a * b == Vector2(a.x * b.x, a.y * b.y)
	void operator*=(const Vector2& value);

	// ベクトルの各軸で割り算
	// a,bというベクトルがあるとすると、a / b == Vector2(a.x / b.x, a.y / b.y)
	void operator/=(const Vector2& value);

	// ベクトルのスケーリング、dxlibのVScaleと同等
	// ベクトルの各軸に同じ値を掛ける。
	// aというベクトル、bという値があるとすると、a * b == Vector2(a.x * b, a.y * b)
	void operator*=(float value);
	void operator*=(int value);

	// ベクトルの各軸に同じ値で割る。
	// aというベクトル、bという値があるとすると、a / b == Vector2(a.x / b, a.y / b)
	void operator/=(float value);
	void operator/=(int value);

	// ベクトルの各軸に同じ値を代入。
	// aというベクトル、bという値があるとすると、a = b == Vector2(a.x = b, a.y = b)
	void operator=(float value);
	void operator=(int value);

	//================================================================================
	// ▼論理

	// 二つのベクトルの値が同じならtrue
	bool operator==(const Vector2& value) const;
	bool operator==(const VECTOR& value) const;

	// 二つのベクトルの値が異なっていればtrue
	bool operator!=(const Vector2& value) const;
	bool operator!=(const VECTOR& value) const;

	//================================================================================
	// ▼各種関数

	//　各要素の合算値を取得する
	inline const float Total() const { return x + y; }

	//　各要素の平均値を取得する
	inline const float Average() const { return Total() / 2.0f; }

	// ベクトルの長さを取得する時、三平方の定理の、最後の平方根を取る部分を飛ばした値。つまり、長さの2乗の値
	// 長さを比較した結果は、長さの2乗を比較した結果と同じなので、球体当たり判定などで長さの比較があるとき代用可能
	// 平方根は結構重い処理なので、代用可能な場所ではSize()よりSquareSize()を使うのが好ましい(学生作品の規模なら誤差な気もするが)
	// dxlibのVSquareSizeと同等
	inline const float SquareSize() const { return x * x + y * y; }

	// ベクトルの長さを取得
	// dxlibのVSizeと同等
	inline const float Size() const { return sqrtf(SquareSize()); }

	// ベクトルの向きはそのまま、長さを1にしたベクトル(正規化ベクトル)を取得
	// ゼロベクトルを入れた場合ゼロベクトルを返す
	// dxlibのVNormと同等
	inline const Vector2 Norm() const {
		float size = Size();
		if (size == 0.0f)
			return Vector2(0.0f, 0.0f);

		return Vector2(x / size, y / size);
	}

	// 引数との差を取得する
	inline const Vector2 Distance(Vector2 value) const { return Vector2(x - value.x, y - value.y); }

	// 引数から自身の方へ向く角度を取得する
	inline const float Direction(Vector2 value) {
		Vector2 v = Distance(value);
		return atan2f(v.y, v.x);
	}

	// ベクトルのサイズが0以上ならTrueを取得する
	inline const bool IsValue() const { return (SquareSize() != 0.0f); }

	// iの値によって要素を取得(0 -> x, 1 -> y)
	// ループ処理で使用
	float Get(int i) const;

	// iの値によって要素にvalueをセット(0 -> x, 1 -> y)
	// ループ処理で使用
	void Set(int i, float value);
};

// Vector2を補助する定数、関数	
namespace V2 {

	//================================================================================
	// ▼定数

	// ゼロベクトル
	static const Vector2 ZERO = Vector2(0.0f);

	// 全て1のベクトル
	static const Vector2 ONE = Vector2(1.0f);

	//================================================================================
	// ▼各種関数

	// X座標の要素だけ設定する
	// そのほかの要素は0になる
	inline const Vector2 SetX(float _x) { return Vector2(_x, 0.0f); }

	// Y座標の要素だけ設定する
	// そのほかの要素は0になる
	inline const Vector2 SetY(float _y) { return Vector2(0.0f, _y); }

	// 二つのベクトルの内積
	inline float Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; }
}
