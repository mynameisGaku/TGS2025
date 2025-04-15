#pragma once
// ◇継承元
#include "object3D.h"

// ◇汎用
#include <string>
#include "config.h"

// ◇個別で必要な物
#include "lightDefine.h"

using namespace LightDefine;

/// <summary>
/// ライトのクラス
/// </summary>
class Light : public Object3D {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	Light(const LightInfo& info);
	virtual ~Light();

	//==========================================================================================
	// ▼各種関数

	virtual void Update() override;

	/// <summary>
	/// ライトに関する更新処理を行う
	/// </summary>
	void LightUpdate();

	/// <summary>
	/// ライトのハンドルを生成する
	/// </summary>
	/// <param name="info">ライトの情報</param>
	void CreateLightHandle(const LightInfo& info);

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 回転角度を設定する(弧度法)
	/// </summary>
	/// <param name="deg">回転角度(弧度法)</param>
	inline void SetDegRotation(const Vector3& deg) { m_rotation = deg; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// ライトの情報を取得する
	/// </summary>
	inline const LightInfo Info() const { return info; }

private:

#ifdef IMGUI

	std::string imguiLabel;	// ImGuiに登録する名前
	void initImGuiNode();			// ImGuiの初期化処理
	void initImGuiNode_LightDir();	// ImGuiの初期化処理(平行光源)
	void initImGuiNode_LightPoint();// ImGuiの初期化処理(点光源)
	void initImGuiNode_LightStop();	// ImGuiの初期化処理(照明光源)

#endif // IMGUI

protected:
	LightInfo info;	// ライトの情報
	Vector3 m_rotation;	// ライトの回転角度(度数法)
	float m_outAngle;	// スポットライトのコーンの外側の角度(度数法)
	float m_inAngle;	// スポットライトのコーンの内側の角度(度数法)

};