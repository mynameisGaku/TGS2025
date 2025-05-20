#pragma once

// Œp³Œ³
#include "Component.h"

// ”Ä—p
#include "../Util/Vector3.h"
#include <list>
#include <string>
#include <assert.h>

class Object3D;

/// <summary>
/// U“®‚Ìî•ñ‚ğ‚ÂƒNƒ‰ƒX
/// </summary>
class Shake : public Component {
public:
	//================================================================================
	// ¥—ñ‹“Œ^

	// U“®‚Ìí—Ş
	enum class Type {
		tNone = -1,
		tVertical,	// c—h‚ê
		tHorizontal,// ‰¡—h‚ê
		tDepth,		// ‰œ—h‚ê
		tAll,		// c+‰¡+‰œ —h‚ê
	};

	//================================================================================
	// ¥ƒRƒ“ƒXƒgƒ‰ƒNƒ^EƒfƒXƒgƒ‰ƒNƒ^

	Shake();
	~Shake();
	
	//================================================================================
	// ¥ŠeíŠÖ”

	void Init(Object3D* parent);
	void Update() override;
	void Draw() override;

	void IntervalUpdate();

	//================================================================================
	// ¥ƒZƒbƒ^[

	/// <summary>
	/// U“®‚ğİ’è‚·‚é
	/// </summary>
	/// <param name="type">U“®‚Ìí—Ş</param>
	/// <param name="power">U“®‚Ì‹­‚³</param>
	/// <param name="duration">Œø‰ÊŠÔ</param>
	void SetParam(std::list<Type> type, float power, Vector3 interval, float duration);

	void ApplyShakeTrs(std::string frameName, float magnification);
	void ClearShakeTrs(std::string frameName);

	//================================================================================
	// ¥ƒQƒbƒ^[

	// U“®‚Ìí—Ş‚ğæ“¾‚·‚é
	inline std::list<Type> GetTypes() const { return types; }
	
	// U“®‚Ì‹­‚³
	inline Vector3 Power() const { return power; }
	
	// U“®‚Ì‹­‚³(‰Šú’l)
	inline Vector3 InitPower() const { return initPower; }
	
	// U“®‚Ì‹­‚³(s—ñ)
	inline MATRIX Matrix() const { return MGetTranslate(power); }

	// Œø‰ÊŠÔ
	inline float Duration() const { return duration; }

	// Œø‰ÊŠÔ(‰Šú’l)
	inline float InitDuration() const { return initDuration; }
	
	// Œø‰ÊŠÔ‚ğ0..1‚Åæ“¾‚·‚é
	inline float CurrentRatio() const {
		if (initDuration == 0)
			assert(0 && "—ëœZ‚Å‚·");
		return duration / initDuration;
	}

	// U“®‚ªI—¹‚µ‚½‚©‚ğæ“¾‚·‚é
	inline bool IsFinish() const { return (duration <= 0.0f); }

private:
	Object3D* parentObj;

	std::list<Type> types;	// U“®‚Ìí—Ş

	Vector3 power;		// U“®‚Ì‹­‚³
	Vector3 initPower;	// U“®‚Ì‹­‚³(Šù’è’l)

	Vector3 interval;		// U“®‚ÌŒü‚«‚ğ•ÏX‚·‚éŠÔŠu
	Vector3 initInterval;	// U“®‚ÌŒü‚«‚ğ•ÏX‚·‚éŠÔŠu(Šù’è’l)
	Vector3 direction;		// U“®‚ÌŒü‚«

	float duration;		// Œø‰ÊŠÔ
	float initDuration;	// Œø‰ÊŠÔ(Šù’è’l)
};