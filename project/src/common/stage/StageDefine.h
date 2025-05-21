#pragma once
#include <string>

namespace StageDefine {

	static const float STAGE_RANGE = 1750.0f;	// ステージの範囲

	/// <summary>
	/// ステージオブジェクトの情報
	/// </summary>
	struct StageObjInfo {
		int hModel;			// 描画モデル
		int hHitModel;		// 当たり判定モデル
		std::string type;	// オブジェクトの種類
		std::string fileName;	// ファイルパス

		StageObjInfo() :
			hModel(-1),
			hHitModel(-1),
			type(""),
			fileName("")
		{
		}
	};

	/// <summary>
	/// ステージオブジェクトの種類
	/// </summary>
	enum class ObjType {
		None = -1,
		Axe_00,
		Max,
	};
}