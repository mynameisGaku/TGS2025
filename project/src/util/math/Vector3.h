#pragma once

#include <stdio.h>
#include <DxLib.h>
#include <string>

class Vector2;
class Vector4;
class Quaternion;
class Matrix;

/**
	@brief	3次元のベクトルを定義します。
*/
class Vector3
{
public:

	float	x;		///< X 要素
	float	y;		///< Y 要素
	float	z;		///< Z 要素

public:

	static const Vector3   Zero;		///< Vector3(0, 0, 0)
	static const Vector3   UnitX;		///< Vector3(1, 0, 0)
	static const Vector3   UnitY;		///< Vector3(0, 1, 0)
	static const Vector3   UnitZ;		///< Vector3(0, 0, 1)
	static const Vector3   Ones;		///< Vector3(1, 1, 1)
	static const Vector3   Horizontal;	///< Vector3(1, 0, 1)

public:

	/**
		@brief	すべての要素を 0.0 に設定してインスタンスを初期化します。
	*/
	Vector3();

	/**
		@brief	指定した値を使用してインスタンスを初期化します。
	*/
	Vector3(float x, float y, float z);

	/**
		@brief	指定した値を使用してインスタンスを初期化します。
	*/
	Vector3(int x, int y, int z);

	/**
		@brief	指定した値を使用してインスタンスを初期化します。
	*/
	Vector3(float xyz);

	/**
		@brief	Vector2 と z 値を指定してインスタンスを初期化します。
	*/
	Vector3(const Vector2& vec, float z);

public:

	/**
		@brief		各要素に値を設定します。
	*/
	void SetElement(float x, float y, float z);

	/**
		@brief		このベクトルの X Y 要素を Vector2 として返します。
	*/
	const Vector2& GetXY() const;

	/**
		@brief		ベクトルの長さを返します。
	*/
	float GetLength() const;

	/**
		@brief		ベクトルの長さの 2 乗を返します。
	*/
	float GetLengthSquared() const;

	/**
		@brief		このベクトルを正規化します。
		@details	ベクトルの長さが 0 の場合は正規化を行いません。
	*/
	Vector3 Normalize() const;

	/**
		@brief		このベクトルを指定された最大値と最小値の範囲にクランプします。
		@param[in]	minVec	: 最小値
		@param[in]	maxVec	: 最大値
	*/
	Vector3 Clamp(const Vector3& minVec, const Vector3& maxVec);

	/**
		@brief		指定された行列を使用してこのベクトルを座標変換します。
		@param[in]	mat		: 処理の基になる行列
		@details	ベクトルを (X, Y, Z, 1.0) として座標変換を行い、結果を w で除算します。
	*/
	Vector3 TransformCoord(const Matrix& mat);

	/**
		@brief		要素のいずれかが NaN または Inf かを判別します。
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "%f, %f, %f\n" を使用します。
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

	// 
	// ループ処理で使用
	/**
		@brief		iの値によって要素を取得
					ループ処理で使用
		@param[in]	i	: （0 -> x, 1 -> y, 2 -> z）
	*/
	float Get(int i) const;

	// iの値によって要素にvalueをセット（0 -> x, 1 -> y, 2 -> z）
	// ループ処理で使用
	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "%f, %f, %f\n" を使用します。
	*/
	void Set(int i, float value);

	/**
		@brief		指定されたクォータニオンを使用してこのベクトルを座標変換します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		変換されたベクトル
	*/
	Vector3 Transform(const Quaternion& qua);

	/**
		@brief		指定された行列を使用してこのベクトルを座標変換します。
		@param[in]	mat		: 処理の基になる行列
		@return		変換されたベクトル
	*/
	Vector3 Transform(const Matrix& mat);

	/* トモミチゾーン */

	// 引数との差を取得する
	inline const Vector3 Distance(const Vector3& value) { return Vector3(x - value.x, y - value.y, z - value.z); }

	// 引数から自身の方へ向く角度を取得する
	inline const float Direction(const Vector3& value) {
		Vector3 v = Distance(value);
		return atan2f(v.x, v.z);
	}

	// ベクトルのサイズが0以上ならTrueを取得する
	inline const bool IsValue() const { return (GetLengthSquared() != 0.0f); }

	// 垂直関係にある？
	inline const bool IsVertical(const Vector3& r) const {
		float d = Dot(*this, r);
		return (-0.000001f < d && d < 0.000001f);	// 誤差範囲内なら垂直と判定
	}

	// 平行関係にある？
	inline const bool IsParallel(const Vector3& r) const {
		Vector3 cross = VCross(*this, r);
		float d = cross.GetLengthSquared();
		return (-0.000001f < d && d < 0.000001f);	// 誤差範囲内なら平行と判定
	}

	// 鋭角関係？
	inline const bool IsSharpAngle(const Vector3& r) const { return (Dot(*this, r) >= 0.0f); }

	//　各要素の合算値を取得する
	inline const float Total() const { return x + y + z; }

	//　各要素の平均値を取得する
	inline const float Average() const { return Total() / 3.0f; }

public:

	/**
		@brief		指定ベクトルを正規化したベクトルを返します。
		@param[in]	x		: 処理の基になるベクトルの X 要素
		@param[in]	y		: 処理の基になるベクトルの Y 要素
		@param[in]	z		: 処理の基になるベクトルの Z 要素
		@return		正規化されたベクトル
	*/
	static Vector3 Normalize(float x, float y, float z);

	/**
		@brief		指定ベクトルを正規化したベクトルを返します。
		@param[in]	vec		: 処理の基になるベクトル
		@return		正規化されたベクトル
	*/
	static Vector3 Normalize(const Vector3& vec);

	/**
		@brief		2つのベクトルの内積を計算します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		2つのベクトルの内積
	*/
	static float Dot(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2つのベクトルの外積を計算します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		2つのベクトルの外積
	*/
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2つのベクトルの最小値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最小値から作成されたベクトル
	*/
	static Vector3 Min(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		2つのベクトルの最大値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最大値から作成されたベクトル
	*/
	static Vector3 Max(const Vector3& vec1, const Vector3& vec2);

	/**
		@brief		入射ベクトルと法線ベクトルから反射ベクトルを計算します。
		@param[in]	vec		: 入射ベクトル
		@param[in]	normal	: 法線ベクトル
		@return		反射ベクトル
	*/
	static Vector3 Reflect(const Vector3& vec, const Vector3& normal);

	/**
		@brief		入射ベクトルと法線ベクトルから滑りベクトルを計算します。
		@param[in]	vec		: 入射ベクトル
		@param[in]	normal	: 法線ベクトル
		@return		滑りベクトル
	*/
	static Vector3 Slide(const Vector3& vec, const Vector3& normal);

	/**
		@brief		指定されたクォータニオンを使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		変換されたベクトル
	*/
	static Vector3 Transform(const Vector3& vec, const Quaternion& qua);

	/**
		@brief		指定された行列を使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	mat		: 処理の基になる行列
		@return		変換されたベクトル
	*/
	static Vector4 Transform(const Vector3& vec, const Matrix& mat);

	/**
		@brief		指定された行列を使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	mat		: 処理の基になる行列
		@details	ベクトルを (X, Y, Z, 1.0) として座標変換を行い、結果を w で除算します。
		@return		変換されたベクトル
	*/
	static Vector3 TransformCoord(const Vector3& vec, const Matrix& mat);
	static Vector3 TransformCoord(const Vector3& vec, const MATRIX& mat);

	/**
		@brief		2 つのベクトル間の線形補間を行います。
		@param[in]	start	: 開始ベクトル (t = 0.0 のときの値)
		@param[in]	end		: 終了ベクトル (t = 1.0 のときの値)
		@param[in]	t		: 加重係数
		@return		補間結果のベクトル
		@details	t は通常、0.0〜1.0 を指定します。
	*/
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

	/**
		@brief		指定されたベクトルを使用して エルミートスプライン補間を実行します。
		@param[in]	v1	: 開始ベクトル
		@param[in]	a1	: 開始ベクトルの接線ベクトル(速度)
		@param[in]	v2	: 終了ベクトル
		@param[in]	a2	: 終了ベクトルの接線ベクトル(速度)
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0〜1.0 を指定します。
	*/
	static Vector3 Hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t);

	/**
		@brief		指定されたベクトルを使用して Catmull-Rom 補間を行います。
		@param[in]	vec1	: 1番目の位置
		@param[in]	vec2	: 2番目の位置 (t = 0.0 のときの値)
		@param[in]	vec3	: 3番目の位置 (t = 1.0 のときの値)
		@param[in]	vec4	: 4番目の位置
		@param[in]	t		: 加重係数
		@return		補間結果のベクトル
		@details	t は通常、0.0〜1.0 を指定します。
	*/
	static Vector3 CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t);

	/**
		@brief		オブジェクト空間(3D空間)のベクトルをスクリーン空間(2D空間)のベクトルに変換する。
		@param[in]	point			: オブジェクト空間上の座標
		@param[in]	worldViewProj	: 結合済みの ワールド - ビュー - プロジェクション行列
		@param[in]	x				: ビューポートの左上 X 座標
		@param[in]	y				: ビューポートの左上 Y 座標
		@param[in]	width			: ビューポートの幅
		@param[in]	height			: ビューポートの高さ
		@param[in]	minZ			: ビューポートの最小深度
		@param[in]	maxZ			: ビューポートの最大深度
	*/
	static Vector3 Project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	/**
		@brief		スクリーン空間(2D空間)のベクトルをオブジェクト空間(3D空間)のベクトルに変換する。
		@param[in]	point			: スクリーン空間上の座標
		@param[in]	worldViewProj	: 結合済みの ワールド - ビュー - プロジェクション行列
		@param[in]	x				: ビューポートの左上 X 座標
		@param[in]	y				: ビューポートの左上 Y 座標
		@param[in]	width			: ビューポートの幅
		@param[in]	height			: ビューポートの高さ
		@param[in]	minZ			: ビューポートの最小深度
		@param[in]	maxZ			: ビューポートの最大深度
	*/
	static Vector3 Unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);

	/**
		@brief		x要素に値をセットし、そのベクトルを返す (x, 0, 0)
		@param[in]	x				: x要素
	*/
	static Vector3 SetX(float x);
	/**
		@brief		x要素に値をセットし、そのベクトルを返す (x, 0, 0)
		@param[in]	x				: x要素
	*/
	static Vector3 SetX(int x);

	/**
		@brief		y要素に値をセットし、そのベクトルを返す (0, y, 0)
		@param[in]	y				: y要素
	*/
	static Vector3 SetY(float y);
	/**
		@brief		y要素に値をセットし、そのベクトルを返す (0, y, 0)
		@param[in]	y				: y要素
	*/
	static Vector3 SetY(int y);

	/**
		@brief		z要素に値をセットし、そのベクトルを返す (0, 0, z)
		@param[in]	z				: z要素
	*/
	static Vector3 SetZ(float z);
	/**
		@brief		z要素に値をセットし、そのベクトルを返す (0, 0, z)
		@param[in]	z				: z要素
	*/
	static Vector3 SetZ(int z);

	// 3次元ベクトルを回転行列に変換する
	inline MATRIX ToRotationMatrix() const {

		MATRIX m = MGetIdent();
		m = MMult(m, MGetRotZ(z));
		m = MMult(m, MGetRotX(x));
		m = MMult(m, MGetRotY(y));

		return m;
	}

public:

	Vector3& operator += (const Vector3& v);
	Vector3& operator += (float v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator -= (float v);
	Vector3& operator *= (const Vector3& v);
	Vector3& operator *= (float v);
	Vector3& operator /= (const Vector3& v);
	Vector3& operator /= (float v);

	friend Vector3 operator + (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator + (const Vector3& v1, float v2);
	friend Vector3 operator + (float v1, const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator - (const Vector3& v1, float v2);
	friend Vector3 operator - (float v1, const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator * (const Vector3& v1, float v2);
	friend Vector3 operator * (float v1, const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1, const Vector3& v2);
	friend Vector3 operator / (const Vector3& v1, float v2);
	friend Vector3 operator / (float v1, const Vector3& v2);

	friend Vector3 operator - (const Vector3& v1);

	bool operator == (const Vector3& v) const;
	bool operator != (const Vector3& v) const;

	// DxLibのVECTORと互換性をもたせる
	Vector3(const VECTOR& v);
	operator const VECTOR() const;
	operator bool() const;
	Vector3 operator + (const VECTOR& v) const;
	Vector3& operator += (const VECTOR& v);
	Vector3 operator - (const VECTOR& v) const;
	Vector3& operator -= (const VECTOR& v);
	Vector3 operator * (const VECTOR& v) const;
	Vector3& operator *= (const VECTOR& v);
	Vector3 operator / (const VECTOR& v) const;
	Vector3& operator /= (const VECTOR& v);


	const friend Vector3 operator *(const Vector3& v, const MATRIX& m1);
	friend Vector3& operator *=(Vector3& v, const MATRIX& m1);
};