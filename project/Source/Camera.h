#pragma once
// ◇継承元
#include "object3D.h"

// ◇汎用
#include "../Library/csvReader.h"

// ◇個別で必要な物
#include "cameraDefine.h"

class StateManager;

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
	/// ステートを変更する処理
	/// </summary>
	/// <param name="id">ステートの番号</param>
	void ChangeState(const CameraDefine::State& id);

	/// <summary>
	/// 地形とめり込まない様にする処理 
	/// </summary>
	void ColCheckToTerrain();

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
	/// 演出を設定する
	/// </summary>
	/// <param name="perfType">演出の種類</param>
	void SetPerformance(const std::string& perfType);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// ステートパターンの情報を取得する
	/// </summary>
	inline StateManager* State() const { return stateManager; }

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

private:
	//================================================================================
	// ▼メンバ変数

	StateManager* stateManager;	// ステートを管理する

	Vector3 offset;	// カメラの相対座標
	Vector3 target;	// カメラの注視点

	const Transform* holder;	// カメラの保有者
	CsvReader* cameraWork;		// カメラ演出情報
};
