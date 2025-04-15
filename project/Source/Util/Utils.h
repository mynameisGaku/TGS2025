#pragma once
#include <DxLib.h>
#include <assert.h>
#include <string>
#include <list>
#include <vector>

#define _OX_EPSILON_ 0.000001f // 誤差

namespace Math {

	//==================================================
	// ▼数学関連

	// 円周率
	static const float PI = 3.1415927f;

	// 円周率の2倍
	static const float PI_TW = PI * 2.0f;

	// 円周率の2乗
	static const float PI_SQ = PI * PI;

	// 重力(2D)
	static const float GRAVITY_2D = 9.8f / 20.0f;

	// 重力(3D)
	static const float GRAVITY_3D = -GRAVITY_2D;

	// 度数法を弧度法(ラジアン)に変換
	// 例：DegToRad(180) = 3.14...
	template<typename T = float>
	inline float DegToRad(T deg) { return static_cast<float>(deg) * PI / 180.0f; }

	// 弧度法(ラジアン)を度数法に変換
	// 例：RadToDeg(3.14) = 180
	template<typename T = float>
	inline float RadToDeg(T rad) { return static_cast<float>(rad) * 180.0f / PI; }

	// 半分にする
	template<typename T = float>
	inline float Half(T value) { return static_cast<float>(value) / 2; }

	// 四半にする
	template<typename T = float>
	inline float Quarter(T value) { return static_cast<float>(value) / 4; }

	/// <summary>
	/// 確率の計算
	/// </summary>
	/// <param name="probability">確率(※百分率)</param>
	/// <returns>命中した場合、True</returns>
	inline bool Probability(int probability) { return GetRand(99) < probability; }

	/// <summary>
	/// 指定した最小値と最大値の間に制限する
	/// </summary>
	/// <param name="v">現在値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>現在値に制限をかけた数値</returns>
	template<typename T = float>
	inline T Clamp(T v, T min, T max) {
		if (v < min)
			return min;
		else if (v > max)
			return max;
		return v;
	}

	inline float RateDecrease(float start, float rate) {

		if (rate >= 1.0f || rate <= 0.0f)
			return start;

		float value = start;
		float ans = 0.0f;
		bool isLoop = true;

		while (isLoop) {
			value *= rate;
			ans += value;

			if (value <= _OX_EPSILON_)
				isLoop = false;
		}

		return ans;
	}
}

namespace String {

	/// <summary>
	/// 一文字分保存する
	/// </summary>
	struct Letter {
		std::string str;	// 文字
		float x;	// 描画座標(X軸)
		float y;	// 描画座標(Y軸)

		Letter() : Letter("", 0.0f, 0.0f) {}

		Letter(const std::string& s, float _x, float _y) :
			str(s),
			x(_x),
			y(_y)
		{
		};
	};

	/// <summary>
	/// フォルダパスの情報を持つ
	/// </summary>
	struct Folder {
		std::string filePath;		// ファイルパス
		std::string resourceName;	// リソース名
		std::string tag;			// タグ

		Folder() :
			filePath(""),
			resourceName(""),
			tag("")
		{
		}

		/// <summary>
		/// フォルダパスの情報を設定する
		/// </summary>
		/// <param name="_filePath">ファイルパス</param>
		/// <param name="resource">リソース名</param>
		/// <param name="_tag">タグ</param>
		Folder(const std::string& _filePath, const std::string& resource, const std::string& _tag) :
			filePath(_filePath),
			resourceName(resource),
			tag(_tag)
		{
		}

		/// <summary>
		/// 指定されたファイルパスと一致しているか
		/// </summary>
		/// <param name="file">ファイルパス</param>
		/// <returns>一致している場合true</returns>
		inline bool IsFilePath(const std::string& file) const { return filePath == file; }

		/// <summary>
		/// 指定されたリソース名と一致しているか
		/// </summary>
		/// <param name="resource">リソース名</param>
		/// <returns>一致している場合true</returns>
		inline bool IsResourceName(const std::string& resource) const { return resourceName == resource; }

		/// <summary>
		/// 指定されたタグと一致しているか
		/// </summary>
		/// <param name="_tag">ファイルパス</param>
		/// <returns>一致している場合true</returns>
		inline bool IsTag(const std::string& _tag) const { return tag == _tag; }
	};
}

namespace Screen {

	//==================================================
	// ▼スクリーン関連

	// 画面の横幅
	static const int WIDTH = 1280;

	// 画面の横幅(float)
	static const float WIDTH_F = 1280.0f;

	// 画面の横幅の半分
	static const float WIDTH_HALF = Math::Half(WIDTH);

	// 画面の縦幅
	static const int HEIGHT = 720;

	// 画面の縦幅(float)
	static const float HEIGHT_F = 720.0f;

	// 画面の縦幅の半分
	static const float HEIGHT_HALF = Math::Half(HEIGHT);
}

namespace Function {

	/// <summary>
	/// ポインターを削除する。nullチェックと、削除後にnullptrを代入してくれる。
	/// </summary>
	/// <param name="p">削除したいポインター</param>
	template<class C>
	inline void DeletePointer(C*& p) {
		if (p != nullptr) {
			delete p;
			p = nullptr;
		}
	}

	/// <summary>
	/// 配列のポインターを削除する。nullチェックと、削除後にnullptrを代入してくれる。
	/// </summary>
	/// <param name="p">削除したいポインター</param>
	template<class C>
	inline void DeletePointerArray(C*& p) {
		if (p != nullptr) {
			delete[] p;
			p = nullptr;
		}
	}

	/// <summary>
	/// 配列のポインターを削除する。nullチェックと、削除後にnullptrを代入してくれる。
	/// </summary>
	/// <param name="p">削除したいポインター</param>
	template<class C>
	inline void DeletePointerList(std::list<C*>& p) {
		if (p.empty())
			return;

		for (auto itr = p.begin(); itr != p.end();) {
			delete* itr;
			itr = p.erase(itr);
		}

		p.clear();
	}

	/// <summary>
	/// 配列のポインターを削除する。nullチェックと、削除後にnullptrを代入してくれる。
	/// </summary>
	/// <param name="p">削除したいポインター</param>
	template<class C>
	inline void DeletePointerVector(std::vector<C*>& p) {
		if (p.empty())
			return;

		for (auto itr = p.begin(); itr != p.end();) {
			delete* itr;
			itr = p.erase(itr);
		}

		p.clear();
	}

	/// <summary>
	/// 角度を徐々に変化させる処理。
	/// 現在の値が終点角度になるまで、回転速度を加算し続ける。
	/// </summary>
	/// <param name="current">現在の値</param>
	/// <param name="terminus">終点角度</param>
	/// <param name="rotSpeed">回転速度</param>
	template<typename T = float>
	inline float RotAngle(T current, T terminus, T rotSpeed) {

		float ans = current;	// 最終的な結果を保持する
		float diffAngle = terminus - current;// 角度の差異

		// マイナスなら一周回してプラスの符号にする
		if (diffAngle < 0)
			diffAngle += Math::PI_TW;

		float angleReverseDif = -diffAngle + Math::PI_TW;	// 逆回転の大きさ(値はプラス)

		// 時計回りの方が近い場合
		if (diffAngle < angleReverseDif) {
			if (diffAngle < rotSpeed)		// 角度の差異と回転速度を比較
				ans = terminus;
			else
				ans += rotSpeed;
		}
		// 反時計回りの方が近い場合
		else {
			if (angleReverseDif < rotSpeed)// 角度の差異と回転速度を比較
				ans = terminus;
			else
				ans -= rotSpeed;
		}

		return ans;
	}

	/// <summary>
	/// フォーマットされた文字列をstd::string型に変換する
	/// </summary>
	inline std::string FormatToString(const char* format, ...) {

		char buffer[1024];  // バッファサイズは必要に応じて調整

		va_list valist;
		va_start(valist, format);
		vsprintf_s(buffer, format, valist);  // バッファにフォーマットされた文字列を書き込む
		va_end(valist);

		return static_cast<std::string>(buffer);
	}

	/// <summary>
	/// 四捨五入を行う
	/// </summary>
	/// <param name="value">四捨五入を行う値</param>
	/// <param name="decimalPoint">小数点何位まで四捨五入するか</param>
	/// <returns>四捨五入を行った値を取得する</returns>
	inline float Rounding(float value, int decimalPoint) {

		float result = 0.0f;

		result = value * static_cast<float>(pow(10, decimalPoint - 1)); //四捨五入したい値を10の(n-1)乗倍する。
		result = roundf(result); //小数点以下を四捨五入する。
		result /= static_cast<float>(pow(10, decimalPoint - 1)); //10の(n-1)乗で割る。
		return result;
	}

	/// <summary>
	/// 桁数を取得する
	/// </summary>
	/// <param name="value">桁数を取得する値</param>
	inline int Digit(int value) {

		int digit = 0;

		if (value == 0)
			return 1;

		while (value != 0) {
			value /= 10;
			digit++;
		}

		return digit;
	}

	/// <summary>
	/// 現在値から終点までを補間する。
	/// </summary>
	/// <param name="current">現在値</param>
	/// <param name="terminus">終点</param>
	/// <param name="rate">倍率</param>
	inline void Interpolation(float* current, float terminus, float rate) {

		const float diff = terminus - *current;
		if (diff == 0.0f)
			return;

		*current += diff * rate;

		bool isPositive = (diff > 0.0f);

		if (isPositive) {
			if (*current >= terminus)
				*current = terminus;
		}
		else {
			if (*current <= terminus)
				*current = terminus;
		}

	}
};