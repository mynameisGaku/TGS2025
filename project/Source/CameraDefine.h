#pragma once

// ◇汎用
#include "../Source/Util/Utils.h"
#include "../Source/Util/Vector3.h"

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

	static const float CAMERA_NEAR = 10.0f;		// 描画処理を行う近辺の位置
	static const float CAMERA_FAR = 100000.0f;	// 描画処理を行う遠方の位置

	static const float CAMERA_PERSPECTIVE = Math::DegToRad(60.0f);	// 視野角
	static const float ROT_SPEED_LIMIT = Math::DegToRad(13.0f);		// カメラの回転速度の制限

	static const float CAMERA_ROT_X_MIN = Math::DegToRad(-85.0f);	// X回転の最小値
	static const float CAMERA_ROT_X_MAX = Math::DegToRad(30.0f);	// X回転の最大値

	static const Vector3 CAMERA_OFFSET_DEF = Vector3(0.0f, 100.0f, -200.0f);	// カメラの既定相対座標
	static const Vector3 CAMERA_TARGET_DEF = Vector3(0.0f, 100.0f, 100.0f);		// カメラの既定注視座標

	static const Vector3 CAMERA_OFFSET_CHASE = Vector3(0.0f, 170.0f, -320.0f);		// 追尾カメラの既定相対座標
	static const Vector3 CAMERA_OFFSET_AIM = Vector3(80.0f, 220.0f, -30.0f);		// エイムカメラの既定相対座標
	static const Vector3 CAMERA_OFFSET_GAME_END = Vector3(-150.0f, 100.0f, -150.0f);// ゲーム終了時のカメラの既定相対座標

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
			target(V3::ZERO),
			position(V3::ZERO),
			holder("")
		{
		}

		~StagingData(){}
	};
}