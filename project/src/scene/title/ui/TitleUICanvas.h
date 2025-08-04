#pragma once
#include <string>
#include "TitleUI.h"

class TitleUIGridCursor;

/// <summary>
/// タイトルUIをまとめるためのクラス
/// </summary>
class TitleUICanvas
{
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
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

	TitleUIGridCursor* GetGridCursor() const { return m_pGridCursor; }
private:
	TitleUIGridCursor*	m_pGridCursor	{};	// グリッドカーソル
	std::list<TitleUI>	m_UIList		{};	// タイトルUIのリスト
	std::string			m_Name			{};	// キャンバスの名前
	bool				m_IsActive		{};	// 有効か？ このCanvasが所有するすべてのUIに影響する。
};