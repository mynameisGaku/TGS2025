#pragma once

// ◇継承元
#include "src/common/component/Component.h"

#include <string>
#include "src/util/input/KeyDefine.h"
#include "src/util/math/vector3.h"

class Chara;

class PlayerController : public Component {
public:
	//=======================================================================================
	// ▼コンストラクタ・デストラクタ

	PlayerController();
	~PlayerController();

	//=======================================================================================
	// ▼各種関数

	void Init(int padNumber);
	void Update() override;

	//=======================================================================================
	// ▼セッター

	/// <summary>
	/// 操作するパッドの番号を設定する
	/// </summary>
	/// <param name="_padNumber">パッドの番号(DX_INPUT_PAD～)</param>
	inline void SetPadNumber(int _padNumber) { padNumber = _padNumber; }

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
	Chara* chara;	// 保有者(CharaBase)

	int padNumber;		// 操作してるパッドの番号
};