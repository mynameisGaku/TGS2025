#pragma once
#include <string>
#include "TitleUI.h"

class TitleUIController;

/// <summary>
/// タイトルUIをまとめるためのクラス
/// </summary>
class TitleUICanvas
{
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(const TitleUIController* pCon);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	void AddUI(const TitleUI& ui)
	{
		m_UIList.push_back(ui);
	}

	TitleUIController*	Controller() const { return m_pController; }

private:
	TitleUIController*	m_pController	{};	// このキャンバスを管理するコントローラー
	std::list<TitleUI>	m_UIList		{};	// タイトルUIのリスト
	std::string			m_Name			{};	// キャンバスの名前
	bool				m_IsActive		{};	// 有効か？ このCanvasが所有するすべてのUIに影響する。
};