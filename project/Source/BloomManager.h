#pragma once
#include "Library/gameObject.h"
#include "BloomRef.h"

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

	void SetDrawScreenToEmitter();
	void SetParameter(BloomRef::Parameter parameter);
private:
	int m_EmitterScreen;	// 個別で発光させたいやつを描画するためのスクリーン

	BloomRef::Parameter m_Parameter;	// ブルームのパラメータ
};