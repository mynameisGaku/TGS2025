#pragma once

// ◇継承元
#include "Component.h"

#include <string>
#include "../KeyDefine.h"
#include "../Util/Vector3.h"

class CharaBase;

class PlayerController : public Component {
public:
	//=======================================================================================
	// ▼コンストラクタ・デストラクタ

	PlayerController();
	~PlayerController();

	//=======================================================================================
	// ▼各種関数

	void Init(int padNumber, float moveSpeed, float rotSpeed);
	void Update() override;

	//=======================================================================================
	// ▼セッター

	/// <summary>
	/// 操作するパッドの番号を設定する
	/// </summary>
	/// <param name="_padNumber">パッドの番号(DX_INPUT_PAD～)</param>
	inline void SetPadNumber(int _padNumber) { padNumber = _padNumber; }

	/// <summary>
	/// 移動速度を設定する
	/// </summary>
	/// <param name="_moveSpeed">移動速度</param>
	inline void SetMoveSpeed(float _moveSpeed) { moveSpeed = _moveSpeed; }

	/// <summary>
	/// 回転速度を設定する
	/// </summary>
	/// <param name="_rotSpeed">回転速度</param>
	inline void SetRotSpeed(float _rotSpeed) { rotSpeed = _rotSpeed; }

	//=======================================================================================
	// ▼ゲッター

	/// <summary>
	/// 操作しているパッド番号を取得する
	/// </summary>
	inline const int PadNumber() const { return padNumber; }

	/// <summary>
	/// 操作しているパッドのスティックの傾きを取得する
	/// </summary>
	Vector3 AnalogStick();

	/// <summary>
	/// ボタンが押されているかを取得する
	/// </summary>
	/// <param name="keyCode">入力種類</param>
	/// <param name="toushPhase">入力状況</param>
	bool IsPressButton(KeyDefine::KeyCode keyCode, KeyDefine::TouchPhase toushPhase);

	/// <summary>
	/// ボタンが押されているかを取得する
	/// </summary>
	/// <param name="input">仮想ボタン名</param>
	/// <param name="toushPhase">入力状況</param>
	bool IsPressButton(const std::string& input, KeyDefine::TouchPhase toushPhase);

	/// <summary>
	/// 操作可能か
	/// </summary>
	bool CanMove() const;

	/// <summary>
	/// 移動入力が行われているか
	/// </summary>
	bool IsMoveButton() const;

private:
	CharaBase* chara;	// 保有者(CharaBase)

	int padNumber;		// 操作してるパッドの番号
	float moveSpeed;	// 移動速度
	float rotSpeed;		// 回転速度
};