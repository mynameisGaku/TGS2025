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
	void OperationByStick(int type = -1);

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 相対座標を設定する
	/// </summary>
	/// <param name="_offset">相対座標</param>
	inline void SetOffset(const Vector3& _offset) { offset = _offset; }

	/// <summary>
	/// 注視点を設定する
	/// </summary>
	/// <param name="_target">注視点を設定する</param>
	inline void SetTarget(const Vector3& _target) { target = _target; }

	/// <summary>
	/// 保有者を設定する
	/// </summary>
	inline void SetHolderTrs(const Transform* trs) { holder = trs; }

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

	CharaBase* TargetChara() const { return targetChara; }

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
	//================================================================================
	// ▼メンバ変数

	TinyFSM<Camera>* fsm;

	Vector3 offset;		// カメラの相対座標
	Vector3 offsetPrev;	// 一つ前のカメラの相対座標
	Vector3 target;		// カメラの注視点
	Vector3 targetPrev;	// 一つ前のカメラの注視点
	ColDefine::Cone cameraCone;

	const Transform* holder;	// カメラの保有者
	CsvReader* cameraWork;		// カメラ演出情報
	int m_CharaIndex;			// キャラクターの番号

	CharaBase* targetChara;
};
