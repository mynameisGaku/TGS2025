#pragma once

// ◇継承元
#include "src/common/component/Component.h"

// ◇汎用
#include "src/util/math/vector3.h"
#include <list>
#include <string>
#include <assert.h>

class Object3D;

/// <summary>
/// 振動の情報を持つクラス
/// </summary>
class Shake : public Component {
public:
	//================================================================================
	// ▼列挙型

	// 振動の種類
	enum class Type {
		tNone = -1,
		tVertical,	// 縦揺れ
		tHorizontal,// 横揺れ
		tDepth,		// 奥揺れ
		tAll,		// 縦+横+奥 揺れ
	};

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	Shake();
	~Shake();
	
	//================================================================================
	// ▼各種関数

	void Init(Object3D* parent);
	void Update() override;
	void Draw() override;

	void IntervalUpdate();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 振動を設定する
	/// </summary>
	/// <param name="type">振動の種類</param>
	/// <param name="power">振動の強さ</param>
	/// <param name="duration">効果時間</param>
	void SetParam(std::list<Type> type, float power, Vector3 interval, float duration);

	void ApplyShakeTrs(std::string frameName, float magnification);
	void ClearShakeTrs(std::string frameName);

	//================================================================================
	// ▼ゲッター

	// 振動の種類を取得する
	inline std::list<Type> GetTypes() const { return types; }
	
	// 振動の強さ
	inline Vector3 Power() const { return power; }
	
	// 振動の強さ(初期値)
	inline Vector3 InitPower() const { return initPower; }
	
	// 振動の強さ(行列)
	inline MATRIX Matrix() const { return MGetTranslate(power); }

	// 効果時間
	inline float Duration() const { return duration; }

	// 効果時間(初期値)
	inline float InitDuration() const { return initDuration; }
	
	// 効果時間を0..1で取得する
	inline float CurrentRatio() const {
		if (initDuration == 0)
			assert(0 && "零除算です");
		return duration / initDuration;
	}

	// 振動が終了したかを取得する
	inline bool IsFinish() const { return (duration <= 0.0f); }

private:
	Object3D* parentObj;

	std::list<Type> types;	// 振動の種類

	Vector3 power;		// 振動の強さ
	Vector3 initPower;	// 振動の強さ(既定値)

	Vector3 interval;		// 振動の向きを変更する間隔
	Vector3 initInterval;	// 振動の向きを変更する間隔(既定値)
	Vector3 direction;		// 振動の向き

	float duration;		// 効果時間
	float initDuration;	// 効果時間(既定値)
};