#pragma once
#include "framework/gameObject.h"
#include "src/reference/bloom/BloomRef.h"

/// <summary>
/// カメラ設定を維持して描画先を変更する
/// </summary>
/// <param name="screen">描画先のスクリーン</param>
void SetDrawScreenWithCamera(int screen);

/// <summary>
/// Draw時に画面にブルームをかける
/// </summary>
/// <author>佐藤紘斗</author>
class BloomManager : public GameObject
{
public:
	BloomManager();
	~BloomManager();
	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// 指定した座標とサイズで分割画面上に描画します
	/// </summary>
	/// <param name="x">描画領域の左上隅のX座標</param>
	/// <param name="y">描画領域の左上隅のY座標</param>
	/// <param name="w">描画領域の幅</param>
	/// <param name="h">描画領域の高さ</param>
	void DrawOnScreenDiv(int x, int y, int w, int h);

	void SetDrawScreenToEmitter();
	void SetDrawScreenToBack();
	void SetParameter(BloomRef::Parameter parameter);
private:
	int m_EmitterScreen;	// 個別で発光させたいやつを描画するためのスクリーン
	bool m_DoBloom;		// ブルームをかけるかどうか
	BloomRef::Parameter m_Parameter;	// ブルームのパラメータ
};