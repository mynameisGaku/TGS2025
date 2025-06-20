#pragma once

// ◇汎用
#include "src/Util/math/Vector3.h"
#include <src/util/time/GameTime.h>

namespace CameraDefine {

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

	enum class CameraAnimState {
		None,			// アニメーションなし
		Begin_To_End,	// 開始位置から終了位置へ移動中
		Hold,			// 終了位置で留まっている
		End_To_Begin,	// 終了位置から開始位置へ戻る
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

	class CameraAnimData {
	public:
		Vector3 begin;			// 開始地点
		Vector3 end;			// 終了地点
		Vector3 beginTarget;	// 開始地点の注視点
		Vector3 endTarget;		// 終了地点の注視点
		Vector3 beginRotMatrix;	// 開始地点の行列
		Vector3 endRotMatrix;	// 終了地点の行列
		float beginToEndSec;	// 開始地点から終了地点までの時間
		float beginToEndSec_Max;// 開始地点から終了地点までの時間
		float endToBeginSec;	// 終了地点から開始地点までの時間
		float endToBeginSec_Max;// 終了地点から開始地点までの時間
		float holdTimeSec;		// 留まる時間

		bool isPlaying;	// アニメーション中かどうか

		bool useAnim;	// アニメーションを使用するかどうか
		bool useTarget;	// 注視点を使用するかどうかs
		bool useMatrix;	// 行列を使用するかどうか

		CameraAnimState state; // アニメーションの状態

		CameraAnimData() : 
			begin(Vector3::Zero),
			end(Vector3::Zero),
			beginTarget(Vector3::Zero),
			endTarget(Vector3::Zero),
			beginRotMatrix(Vector3::Zero),
			endRotMatrix(Vector3::Zero),
			beginToEndSec(0.0f),
			beginToEndSec_Max(0.0f),
			endToBeginSec(0.0f),
			endToBeginSec_Max(0.0f),
			holdTimeSec(0.0f),
			isPlaying(false),
			useAnim(false),
			useTarget(false),
			useMatrix(false),
			state(CameraAnimState::None)
		{
		}

		~CameraAnimData() {}

		inline void SetAnim(const Vector3& begin, const Vector3& end, float beginToEnd, float endToBegin) {
			this->begin = begin;
			this->end = end;
			this->beginToEndSec = beginToEnd;
			this->beginToEndSec_Max = beginToEnd;
			this->endToBeginSec = endToBegin;
			this->endToBeginSec_Max = endToBegin;

			isPlaying = true; // アニメーション開始
			useAnim = true; // アニメーションを使用する
			state = CameraAnimState::Begin_To_End; // 開始位置から終了位置へ移動中
		}

		inline void SetRotMat(const Vector3& begin, const Vector3& end, float beginToEnd, float endToBegin) {
			this->beginRotMatrix = begin;
			this->endRotMatrix = end;
			this->beginToEndSec = beginToEnd;
			this->beginToEndSec_Max = beginToEnd;
			this->endToBeginSec = endToBegin;
			this->endToBeginSec_Max = endToBegin;

			isPlaying = true; // アニメーション開始
			useMatrix = true; // アニメーションを使用する
			state = CameraAnimState::Begin_To_End; // 開始位置から終了位置へ移動中
		}
		
		inline void SetHoldTime(float sec) {
			this->holdTimeSec = sec;
		}

		inline void SetUseAnim(bool use) {
			this->useAnim = use;
		}

		inline void SetUseTarget(bool use) {
			this->useTarget = use;
		}

		inline void SetUseMatrix(bool use) {
			this->useMatrix = use;
		}

		inline void Reset() {
			this->begin = Vector3::Zero;
			this->end = Vector3::Zero;
			this->beginTarget = Vector3::Zero;
			this->endTarget = Vector3::Zero;
			this->beginRotMatrix = Vector3::Zero;
			this->endRotMatrix = Vector3::Zero;
			this->beginToEndSec = 0.0f;
			this->beginToEndSec_Max = 0.0f;
			this->endToBeginSec = 0.0f;
			this->endToBeginSec_Max = 0.0f;
			this->holdTimeSec = 0.0f;
			this->isPlaying = false;
			this->useAnim = false;
			this->useTarget = false;
			this->useMatrix = false;
			this->state = CameraAnimState::None;
		}

		inline void Update() {

			if (not isPlaying)
				return;

			switch (state) {
			case CameraDefine::CameraAnimState::Begin_To_End:
				if (beginToEndSec > 0.0f) {
					beginToEndSec -= GTime.deltaTime;
				}
				else {
					beginToEndSec = 0.0f;
					state = CameraAnimState::Hold;
				}
				break;

			case CameraDefine::CameraAnimState::Hold:
				if (holdTimeSec > 0.0f) {
					holdTimeSec -= GTime.deltaTime;
				}
				else {
					holdTimeSec = 0.0f;
					state = CameraAnimState::End_To_Begin;
				}
				break;

			case CameraDefine::CameraAnimState::End_To_Begin:
				if (endToBeginSec > 0.0f) {
					endToBeginSec -= GTime.deltaTime;
				}
				else {
					endToBeginSec = 0.0f;
					state = CameraAnimState::None;
					isPlaying = false; // アニメーション終了
				}
				break;

			default:
				break;
			}
		}
	};
}