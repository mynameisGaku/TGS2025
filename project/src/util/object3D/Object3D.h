#pragma once
#include "framework/gameObject.h"
#include "framework/myDxLib.h"
#include "src/util/transform/Transform.h"

/// <summary>
/// 3Dオブジェクトに関する既定クラス
/// <remarks>トランスフォームやモデルの情報を保持している</remarks>
/// </summary>
class Object3D : public GameObject {
public:
	//==========================================================================================
	// ▼public変数

	Transform* transform;	// 座標・回転・拡縮の情報

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	Object3D();

	/// <summary>
	/// 引数のTranformで初期化を行う
	/// </summary>
	/// <param name="transform">座標・回転・拡縮の情報</param>
	Object3D(const Transform& transform);


	/// <summary>
	/// 引数のTranformで初期化を行う
	/// </summary>
	/// <param name="model">描画モデル</param>
	/// <param name="transform">座標・回転・拡縮の情報</param>
	Object3D(const int& model, const Transform& transform);
	
	virtual ~Object3D();

	//==========================================================================================
	// ▼各種関

	virtual void Draw() override;

	/// <summary>
	/// 座標軸線を描画する(※デバッグ機能)
	/// </summary>
	void DrawAixs();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 座標・回転・拡縮の情報を設定する
	/// </summary>
	void SetTransform(const Transform& trs);

	/// <summary>
	/// モデルを設定する
	/// </summary>
	/// <param name="handle">モデルのハンドル</param>
	void SetModel(const int& handle);

	/// <summary>
	/// モデルを描画するか設定する
	/// </summary>
	inline void SetModelDraw(const bool& value) { isModelDraw = value; }

	/// <summary>
	/// 座標軸線を描画するか設定する
	/// </summary>
	inline void SetAxisDraw(const bool& value) { isAxisDraw = value; }

	inline void SetLocalMatrix(const MATRIX& mat) { localMatrix = mat; }

	//==========================================================================================
	// ▼ゲッター

	// モデルデータを取得する
	inline const int Model() const { return hModel; }

	// モデルを描画するか取得する
	inline bool IsModelDraw() const { return isModelDraw; }

	// 座標軸線を描画するか取得する
	inline bool IsAxisDraw() const { return isAxisDraw; }

protected:
	//==========================================================================================
	// ▼メンバ変数

	int hModel;			// モデルのハンドル

	MATRIX localMatrix;

	bool isModelDraw;	// モデルの描画の有無
	bool isAxisDraw;	// 座標軸線の描画の有無

};