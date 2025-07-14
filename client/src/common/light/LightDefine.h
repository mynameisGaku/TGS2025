#pragma once
#include "framework/myDxLib.h"

namespace LightDefine {

	// ライトの種類
	enum class LightType {
		None = -1,
		Directional,	// 平行光源
		Point,			// 点光源
		Spot,			// 照明光源
		Ambient,		// 環境光源
		Max
	};

	/// <summary>
	/// ライトで使用する情報
	/// </summary>
	class LightInfo {
	public:
		LightType type;		// ライトの種類
		int handle;			// ライトのハンドル
		float range;		// ライトの有効距離
		float atten0;		// 距離減衰パラメーター0
		float atten1;		// 距離減衰パラメーター1
		float atten2;		// 距離減衰パラメーター2
		float outAngle;		// スポットライトのコーンの外側の角度
		float inAngle;		// スポットライトのコーンの内側の角度
		COLOR_F colorDif;	// 拡散色	
		COLOR_F colorSpc;	// 反射色
		COLOR_F colorAmb;	// 環境色

		/// <summary>
		/// 初期化
		/// </summary>
		LightInfo() :
			type(LightType::None),
			handle(-1),
			range(0.0f),
			atten0(0.0f),
			atten1(0.0f),
			atten2(0.0f),
			outAngle(0.0f),
			inAngle(0.0f),
			colorDif(COLOR_F(1.0f, 1.0f, 1.0f, 1.0f)),
			colorSpc(COLOR_F(1.0f, 1.0f, 1.0f, 1.0f)),
			colorAmb(COLOR_F(0.0f, 0.0f, 0.0f, 0.0f))
		{
		}
		
		/// <summary>
		/// 引数で初期化
		/// </summary>
		/// <param name="_type">種類名</param>
		/// <param name="_handle">ライトのハンドル</param>
		/// <param name="_range">ライトの有効距離</param>
		/// <param name="_atten0">距離減衰パラメーター0</param>
		/// <param name="_atten1">距離減衰パラメーター1</param>
		/// <param name="_atten2">距離減衰パラメーター2</param>
		/// <param name="_outAngle">スポットライトのコーンの外側の角度</param>
		/// <param name="_inAngle">スポットライトのコーンの内側の角度</param>
		/// <param name="_colorDif">拡散色</param>
		/// <param name="_colorSpc">反射色</param>
		/// <param name="_colorAmb">環境色</param>
		LightInfo(const LightType& _type, const int& _handle, const float& _range,
				  const float& _atten0, const float& _atten1, const float& _atten2, const float& _outAngle, const float& _inAngle,
				  const COLOR_F& _colorDif, const COLOR_F& _colorSpc, const COLOR_F& _colorAmb) :
			type(_type),
			handle(_handle),
			range(_range),
			atten0(_atten0),
			atten1(_atten1),
			atten2(_atten2),
			outAngle(_outAngle),
			inAngle(_inAngle),
			colorDif(_colorDif),
			colorSpc(_colorSpc),
			colorAmb(_colorAmb)
		{
		}

		~LightInfo() {}
	};
}