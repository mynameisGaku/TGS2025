#pragma once
#include "../Library/gameObject.h"
#include "../Library/myDxLib.h"
#include "../Source/Util/RectTransform.h"

/// <summary>
/// 2Dオブジェクトに関する既定クラス
/// <remarks>トランスフォームやハンドラの情報を保持している</remarks>
/// </summary>
class Object2D : public GameObject {
public:
	//==========================================================================================
	// ▼public変数

	RectTransform* rectTransform;	// 座標・回転・拡縮の情報

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	Object2D();

	/// <summary>
	/// 引数のRectTranformで初期化を行う
	/// </summary>
	/// <param name="rectTransform">座標・回転・拡縮の情報</param>
	Object2D(const RectTransform& rectTransform);

	/// <summary>
	/// 引数のRectTranformで初期化を行う
	/// </summary>
	/// <param name="image">描画画像</param>
	/// <param name="rectTransform">座標・回転・拡縮の情報</param>
	Object2D(const int& image, const RectTransform& rectTransform);

	virtual ~Object2D();

	//==========================================================================================
	// ▼各種関数

	virtual void Draw() override;

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 座標・回転・拡縮の情報を設定する
	/// </summary>
	void SetTransform(const RectTransform& trs);

	/// <summary>
	/// 画像を設定する(その他の設定は変更しない)
	/// </summary>
	/// <param name="handle">画像のハンドル</param>
	void SetImage(const int& handle);

	/// <summary>
	/// 画像を設定する(その他の設定も変更する)
	/// </summary>
	/// <param name="handle">画像のハンドル</param>
	/// <param name="clipStart">切り抜き開始地点</param>
	/// <param name="clipSize">切り抜きの大きさ</param>
	/// <param name="center">画像の中心点</param>
	void SetImage(const int& handle, const Vector2& clipStart, const Vector2& clipSize, const Vector2& center);

	/// <summary>
	/// 切り抜き開始点を設定する
	/// </summary>
	/// <param name="value">切り抜きを開始する座標(左上)</param>
	inline void SetClipStart(const Vector2& value) { clipStart = value; }

	/// <summary>
	/// 切り抜きの大きさを設定する
	/// </summary>
	/// <param name="value">切り抜きの大きさ</param>
	inline void SetClipSize(const Vector2& value) { clipSize = value; }

	/// <summary>
	/// 描画時、画像のどの位置を中心とするかを設定する(0..1)
	/// </summary>
	/// <param name="value">0.5fの場合、画像の中心を描画時の中心とする</param>
	inline void SetCenter(const Vector2& value) { center = value; }

	//==========================================================================================
	// ▼ゲッター

	// 画像データを取得する
	inline int Image() const { return hImage; }

	// 拡大率を含めた、画面に対する画像の描画サイズを取得する
	inline Vector2 ImageSize() const { return Vector2(imageSize.x * rectTransform->scale.x, imageSize.y * rectTransform->scale.y); }

	// アンカーポイントによるズレ値を取得する
	virtual Vector2 DisplacementByAnchorPoint();

protected:
	//==========================================================================================
	// ▼メンバ変数

	int hImage;			// 画像のハンドル

	Vector2 imageSize;	// 画像サイズ
	Vector2 clipStart;	// 切り抜き開始点
	Vector2 clipSize;	// 切り抜きの大きさ
	Vector2 center;		// 描画時、画像のどの位置を中心とするかを設定する(0..1)
};