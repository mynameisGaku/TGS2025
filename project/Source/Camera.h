#pragma once
// ◇継承元
#include "object3D.h"

// ◇汎用
#include "../Library/csvReader.h"
#include "Source/TinyFSM.h"

// ◇個別で必要な物
#include "cameraDefine.h"

class StateManager;

namespace
{
	static const float MOVE_SPEED = 12.0f;	// 横移動速度
	static const float SHIFT_SPEED = 6.0f;	// 縦移動速度
	static const float ROT_SPEED = Math::DegToRad(3.0f);	// 回転速度
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

	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// <para>ステートを変更する処理</para>
	/// <para>引数の先頭に"＆"をつけるのを忘れないでください。</para>
	/// </summary>
	/// <param name="state">ステートの関数ポインタ</param>
	void ChangeState(void(Camera::*state)(FSMSignal));

	// 地形とめり込まない様にする処理 
	void ColCheckToTerrain();

	//================================================================================
	// ▼セッター

	// 相対座標を設定する
	inline void SetOffset(const Vector3& _offset) { offset = _offset; }

	// 保有者を設定する
	inline void SetHolderTrs(const Transform* trs) { holder = trs; }

	// 追従するキャラクターの番号を設定する
	inline void SetHolderCharaIndex(int index) { m_CharaIndex = index; }

	//================================================================================
	// ▼ゲッター

	// ステートパターンの情報を取得する
	inline TinyFSM<Camera>* State() const { return fsm; }

	// 相対座標を取得する
	inline Vector3 Offset() const { return offset; }

	// 回転行列をかけた相対座標を取得する
	inline Vector3 OffsetRotAdaptor() const { return offset * transform->Global().RotationMatrix(); } 

	// カメラの保有者のトランスフォームを取得する
	inline const Transform* HolderTrs() const { return holder; }

	// カメラの位置(絶対座標)
	Vector3 WorldPos() const;

	// 注視点へ向かうベクトル
	Vector3 TargetLay() const;

	//================================================================================
	// ▼ステート

	void DebugState(FSMSignal sig);
	void ChaseState(FSMSignal sig);
	void AimState(FSMSignal sig);

public:
	Vector3 offset;		// カメラの相対座標
private:

	TinyFSM<Camera>* fsm;	// ステートマネージャー

	Vector3 offsetPrev;	// 一つ前のカメラの相対座標

	const Transform* holder;	// カメラの保有者
	int m_CharaIndex;			// キャラクターの番号

	// 移動処理
	void moveProcess();

	/// <summary>
	/// マウスによるカメラ操作
	/// </summary>
	/// <param name="type">【0】の場合、X軸のみ。【1】の場合、Y軸のみ。【その他】XY軸。
	/// </param>
	void operationByMouse(int type = -1);

	/// <summary>
	/// スティックによるカメラ操作
	/// </summary>
	/// <param name="type">【0】の場合、X軸のみ。【1】の場合、Y軸のみ。【その他】XY軸。
	/// </param>
	void operationByStick(int type = -1);
};
