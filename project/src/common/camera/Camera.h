#pragma once
// ◇継承元
#include "src/util/object3D/Object3D.h"

// ◇汎用
#include "src/util/file/csv/csvReader.h"
#include "src/util/fsm/TinyFSM.h"
#include "src/util/math/MathUtil.h"

// ◇個別で必要な物
#include "src/common/camera/CameraDefine.h"
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/component/shake/Shake.h"
#include "src/util/math/Vector2.h"

class StateManager;
class CharaBase;

/// <summary>
/// カメラのクラス
/// </summary>
class Camera : public Object3D {
public:

	/// <summary>
	/// カメラの視点移動方法
	/// </summary>
	enum class ViewPointShift {
		None = -1,	// 無
		Horizontal,	// 水平軸のみ
		Vertical,	// 垂直軸のみ
		All			// 水平軸・垂直軸を含む
	};

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	Camera();
	~Camera();
	
	//================================================================================
	// ▼各種関数

	/// <summary>
	/// 再初期化
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// <para>ステートを変更する処理</para>
	/// <para>引数の先頭に"＆"をつけるのを忘れないでください。</para>
	/// </summary>
	/// <param name="state">ステートの関数ポインタ</param>
	void ChangeState(void(Camera::*state)(FSMSignal));

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 相対座標を設定する
	/// </summary>
	/// <param name="_offset">相対座標</param>
	inline void SetOffset(const Vector3& _offset) { m_Offset = _offset; }

	/// <summary>
	/// 注視点を設定する
	/// </summary>
	/// <param name="_target">注視点を設定する</param>
	inline void SetTarget(const Vector3& _target) { m_Target = _target; }

	/// <summary>
	/// 保有者を設定する
	/// </summary>
	inline void SetHolderTrs(const Transform* trs) { m_pHolder = trs; }

	/// <summary>
	/// 描画を行うかどうかを設定する
	/// </summary>
	inline void SetIsView(bool view) { m_IsView = view; }

	/// <summary>
	/// 追従するキャラクターの番号を設定する
	/// </summary>
	/// <param name="index">キャラクターの番号</param>
	inline void SetHolderCharaIndex(int index) { m_CharaIndex = index; }

	/// <summary>
	/// 演出を設定する
	/// </summary>
	/// <param name="perfType">演出の種類</param>
	void SetPerformance(const std::string& perfType);

	/// <summary>
	/// アニメーションを設定する。開始地点から終了地点へ移動して、終了地点から開始地点へ戻る動作を、指定された秒数で行う
	/// </summary>
	void SetAnimation(const CameraDefine::CameraAnimData& animData);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ステートパターンの情報を取得する
	/// </summary>
	inline TinyFSM<Camera>* State() const { return m_Fsm; }

	/// <summary>
	/// 相対座標を取得する
	/// </summary>
	inline const Vector3 Offset() const { return m_Offset; }

	/// <summary>
	/// 回転行列をかけた相対座標を取得する
	/// </summary>
	inline const Vector3 OffsetRotAdaptor() const { return m_Offset * transform->Global().RotationMatrix(); }

	/// <summary>
	/// 注視点を取得する
	/// </summary>
	inline const Vector3 Target() const { return m_Target; }

	/// <summary>
	/// カメラの保有者のトランスフォームを取得する
	/// </summary>
	/// <returns></returns>
	inline const Transform* HolderTrs() const { return m_pHolder; }

	/// <summary>
	/// カメラワークの情報を取得する
	/// </summary>
	/// <returns>カメラワーク情報のCsvデータ</returns>
	inline const CsvReader* CameraWorkData() const { return m_CameraWork; }

	/// <summary>
	/// カメラの位置(絶対座標)
	/// </summary>
	const Vector3 WorldPos() const;

	/// <summary>
	/// 注視点へ向かうベクトル
	/// </summary>
	const Vector3 TargetLay() const;

	/// <summary>
	/// 注視しているキャラクター
	/// </summary>
	inline const CharaBase* TargetChara() const { return m_pTargetChara; }

	/// <summary>
	/// 描画が終了したかどうかを判定
	/// </summary>
	inline bool IsDrawEnd() const { return m_DrawFlag; }

	/// <summary>
	/// 描画を行うか
	/// </summary>
	inline bool IsView() const { return m_IsView; }

	/// <summary>
	/// カメラの正面にいるかどうかを返す
	/// </summary>
	bool IsFrontView(const Vector3& pos) const;

	/// <summary>
	/// カメラの右側にいるかどうかを返す
	/// </summary>
	bool IsRightView(const Vector3& pos) const;

	//================================================================================
	// ▼ステート

	/// <summary>
	/// デバッグステート
	/// </summary>
	void DebugState(FSMSignal sig);

	/// <summary>
	/// 追従ステート
	/// </summary>
	void ChaseState(FSMSignal sig);

	/// <summary>
	/// 狙うステート
	/// </summary>
	void AimState(FSMSignal sig);

private:

	/// <summary>
	/// カメラ描画処理
	/// </summary>
	void rendering();

	/// <summary>
	/// 地形とめり込まない様にする処理 
	/// </summary>
	void colCheckToTerrain();

	/// <summary>
	/// 移動処理
	/// </summary>
	void moveProcess();

	/// <summary>
	/// マウスによるカメラ操作
	/// </summary>
	/// <param name="type">カメラの視点移動方法</param>
	void operationByMouse(ViewPointShift type);

	/// <summary>
	/// スティックによるカメラ操作
	/// </summary>
	/// <param name="padNumber">パッドの番号</param>
	/// <param name="type">カメラの視点移動方法</param>
	void operationByStick(int padNumber, ViewPointShift type);

	/// <summary>
	/// 演出の更新処理
	/// </summary>
	void updateAnimation();

	/// <summary>
	/// 仮想カメラの描画位置をカプセルで描画
	/// </summary>
	void drawVirtualCamera();

	//================================================================================
	// ▼メンバ変数

	TinyFSM<Camera>* m_Fsm;

	Vector3 m_PositionPrev;	// 補間前のカメラの座標
	Vector3 m_RotationPrev;	// 補間前のカメラの回転

	Vector3 m_Offset;		// カメラの相対座標
	Vector3 m_OffsetPrev;	// 補間前のカメラの相対座標
	
	Vector3 m_Target;		// カメラの注視点
	Vector3 m_TargetPrev;	// 補間前のカメラの注視点

	MATRIX m_CameraRotMat;	// カメラの回転行列

	ColDefine::Cone m_CameraCone;	// カメラの視野角情報

	CameraDefine::CameraAnimData m_AnimData;	// カメラアニメーションのデータ

	Shake* m_pShake;	// シェイクコンポーネント

	CsvReader* m_CameraWork;	// カメラ演出情報
	const Transform* m_pHolder;	// カメラの保有者
	int m_CharaIndex;			// キャラクターの番号

	float m_EasingTime;				// イージング用タイマー
	float m_TargetTransitionTime;	// 注視しているキャラに引っ付くまでの時間

	bool m_IsView;		// 描画しているか
	bool m_DrawFlag;	// 描画が完了しているか

	const CharaBase* m_pFollowerChara;	// 追尾しているキャラ
	const CharaBase* m_pTargetChara;	// 注視しているキャラ
};
