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

class StateManager;
class CharaBase;

namespace
{
	static const float MOVE_SPEED = 12.0f;	// 横移動速度
	static const float SHIFT_SPEED = 6.0f;	// 縦移動速度
	static const float ROT_SPEED = MathUtil::ToRadians(3.0f);	// 回転速度
}

/// <summary>
/// カメラのクラス
/// </summary>
class Camera : public Object3D {
public:
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

	/// <summary>
	/// 地形とめり込まない様にする処理 
	/// </summary>
	void ColCheckToTerrain();

	/// <summary>
	/// 移動処理
	/// </summary>
	void MoveProcess();

	/// <summary>
	/// マウスによるカメラ操作
	/// </summary>
	/// <param name="type">【0】の場合、X軸のみ。【1】の場合、Y軸のみ。【その他】XY軸。
	/// </param>
	void OperationByMouse(int type = -1);

	/// <summary>
	/// スティックによるカメラ操作
	/// </summary>
	/// <param name="type">【0】の場合、X軸のみ。【1】の場合、Y軸のみ。【その他】XY軸。
	/// </param>
	void OperationByStick(int padNumber, int type = -1);

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 相対座標を設定する
	/// </summary>
	/// <param name="_offset">相対座標</param>
	inline void SetOffset(const Vector3& _offset) { offset = _offset; }

	/// <summary>
	///補間先の相対座標を設定する
	/// </summary>
	/// <param name="_offset">相対座標</param>
	inline void SetOffsetAfter(const Vector3& _offset) { offsetAfter = _offset; }

	/// <summary>
	/// 注視点を設定する
	/// </summary>
	/// <param name="_target">注視点を設定する</param>
	inline void SetTarget(const Vector3& _target) { target = _target; }

	/// <summary>
	/// 補間先の注視点を設定する
	/// </summary>
	/// <param name="_target">注視点を設定する</param>
	inline void SetTargetAfter(const Vector3& _target) { targetAfter = _target; }

	/// <summary>
	/// 保有者を設定する
	/// </summary>
	inline void SetHolderTrs(const Transform* trs) { holder = trs; }

	/// <summary>
	/// 描画を行うかどうかを設定する
	/// </summary>
	inline void SetIsView(bool view) { isView = view; }

	/// <summary>
	/// クラスを基に保有者を設定する
	/// </summary>
	/// <typeparam name="C">保有させたいクラス</typeparam>
	template<class C>
	inline void SetHolder() {

		C* c = FindGameObject<C>();
		if (c == nullptr) {
			SetHolderTrs(nullptr);
			return;
		}

		Object3D* obj = dynamic_cast<Object3D*>(c);
		if (obj == nullptr) {
			SetHolderTrs(nullptr);
			return;
		}

		SetHolderTrs(obj->transform);
	}

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
	/// アニメーションを設定して、開始地点から終了地点へ移動して、終了地点から地点へ戻る動作を、指定された秒数で行う
	/// </summary>
	/// <param name="begin">アニメーションの開始位置を表すVector3。</param>
	/// <param name="end">アニメーションの終了位置を表すVector3。</param>
	/// <param name="target">アニメーションのターゲット位置を表すVector3。</param>
	/// <param name="sec">アニメーションの効果時間(秒)</param>
	void SetAnimation(const Vector3& begin, const Vector3& end, const Vector3& target, float sec);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ステートパターンの情報を取得する
	/// </summary>
	inline TinyFSM<Camera>* State() const { return fsm; }

	/// <summary>
	/// 相対座標を取得する
	/// </summary>
	inline Vector3 Offset() const { return offset; }

	/// <summary>
	///相対座標のアドレスを取得する 
	/// </summary>
	inline Vector3& OffsetPtr() { return offset; }

	/// <summary>
	/// 回転行列をかけた相対座標を取得する
	/// </summary>
	inline Vector3 OffsetRotAdaptor() const { return offset * transform->Global().RotationMatrix(); } 

	/// <summary>
	/// 注視点を取得する
	/// </summary>
	inline Vector3 Target() const { return target; }

	/// <summary>
	/// 注視点のアドレスを取得する
	/// </summary>
	inline Vector3& TargetPtr() { return target; }

	/// <summary>
	/// カメラの保有者のトランスフォームを取得する
	/// </summary>
	/// <returns></returns>
	inline const Transform* HolderTrs() const { return holder; }

	/// <summary>
	/// カメラワークの情報を取得する
	/// </summary>
	/// <returns>カメラワーク情報のCsvデータ</returns>
	inline CsvReader* CameraWorkData() const { return cameraWork; }

	/// <summary>
	/// カメラの位置(絶対座標)
	/// </summary>
	Vector3 WorldPos() const;

	/// <summary>
	/// 注視点へ向かうベクトル
	/// </summary>
	Vector3 TargetLay() const;

	/// <summary>
	/// 注視しているキャラクター
	/// </summary>
	const CharaBase* TargetChara() const { return m_TargetChara; }

	/// <summary>
	/// 描画を行うか
	/// </summary>
	inline bool IsView() const { return isView; }

	/// <summary>
	/// アニメーションが再生中か
	/// </summary>
	inline bool IsPlayAnim() const { return m_AnimationSec > 0.0f; }

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

	void UpdateOffsetLeap();
	void UpdateTargetLeap();
	void UpdateAnimation();

	void DrawVirtualCamera();

	//================================================================================
	// ▼メンバ変数

	TinyFSM<Camera>* fsm;

	Vector3 offset;		// カメラの相対座標
	Vector3 offsetAfter;// 補間先のカメラの相対座標
	Vector3 target;		// カメラの注視点
	Vector3 targetAfter;// 補間先のカメラの注視点
	ColDefine::Cone cameraCone;

	Vector3 m_AnimationBegin;	// アニメーション開始位置
	Vector3 m_AnimationEnd;		// アニメーション終了位置
	Vector3 m_AnimationTarget;	// アニメーションの注視点
	float m_AnimationSec;		// アニメーションの時間
	float m_AnimationSecMax;	// アニメーションの最大時間

	Shake* m_pShake;	// シェイクコンポーネント

	const Transform* holder;	// カメラの保有者
	CsvReader* cameraWork;		// カメラ演出情報
	int m_CharaIndex;			// キャラクターの番号
	float m_TargetTransitionTime;
	bool isView;
	bool drawFlag;

	const CharaBase* m_TargetChara;
};
