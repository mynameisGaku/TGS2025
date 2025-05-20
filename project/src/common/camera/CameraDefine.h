#pragma once

// ◇汎用
#include "src/Util/Utils.h"
#include "src/Util/math/Vector3.h"

namespace CameraDefine {

	/// <summary>
	/// カメラの各種ステート
	/// </summary>
	enum State {
		sNone = -1,
		sTitle,			// タイトル
		sPlay,			// プレイ
		sResult,		// リザルト
		sChase,
		sTarget,		// 注視
		sPerformance,	// 演出
		sTeam,			// チームプレイ
		sDebug,			// デバッグ
		sMax
	};

	static const float SHAKE_MIN = 4.0f;	// カメラ振動の最小値
	static const float SHAKE_MAX = 10.0f;	// カメラ振動の最小値

	/// <summary>
	/// カメラ演出の種類
	/// </summary>
	enum class CameraWorkType {
		None = -1,
		Type,			// 種類
		Opening,		// 開始時
		PlayerLose,		// 敗北
		PlayerWin,		// 勝利
		PlayerAtkSp,	// プレイヤーの必殺技
		Max
	};

	/// <summary>
	/// カメラ演出情報の要素
	/// </summary>
	enum class CameraWorkElement {
		None = -1,
		Type,		// 種類
		Duration,	// 効果時間
		TargetX,	// 注視点(X軸)
		TargetY,	// 注視点(Y軸)
		TargetZ,	// 注視点(Z軸)
		PositionX,	// 座標(X軸)
		PositionY,	// 座標(Y軸)
		PositionZ,	// 座標(Z軸)
		AffterTaste,// 余韻
		Holder,		// カメラの保有者
		Max
	};

	/// <summary>
	/// カメラ演出の補間で使用する情報
	/// </summary>
	class StagingData {
	public:
		float duration;		// 効果時間
		float totalDuration;// 総効果時間
		float affterTaste;	// 余韻
		bool end;			// 終了したか
		Vector3 position;	// 座標
		Vector3 target;		// 注視点
		std::string holder;	// 保有者

		StagingData() :
			duration(0.0f),
			totalDuration(0.0f),
			affterTaste(0.0f),
			end(false),
			target(Vector3::Zero),
			position(Vector3::Zero),
			holder("")
		{
		}

		~StagingData(){}
	};
}