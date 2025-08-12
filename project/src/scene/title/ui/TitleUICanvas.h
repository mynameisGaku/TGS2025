#pragma once
#include <string>
#include "TitleUI.h"
#include <src/util/math/Vector2.h>

class TitleUIController;

struct UI_TITLE_CANVAS_DESC
{
	std::string		NAME{};
	Vector2			BEGIN{};
	Vector2			END{};
	bool			IS_FIT_SCREEN{};
	bool			IS_DEFAULT_ACTIVATE{};
};

/// <summary>
/// タイトルUIをまとめるためのクラス
/// </summary>
class TitleUICanvas
{
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(const UI_TITLE_CANVAS_DESC& desc, TitleUIController* pCon);
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

	TitleUI GetUI(const std::string& name);

	void Activate() { m_IsActive = true; }
	void Deactivate() { m_IsActive = false; }
	bool IsActive() const { return m_IsActive; }

	Vector2 GetPoint(const TUI_CANVAS_ANCHOR_POINT& anchor);
	const Vector2 GetBegin() const;
	const Vector2 GetEnd() const;

	float ScaleX() const;
	float ScaleY() const;
	Vector2 CanvasOriginOnScreen() const;
	Vector2 CanvasDesignSize() const;

private:
	void calcMapping(Vector2& origin, float& sx, float& sy) const;
	Vector2 m_Begin;
	Vector2 m_End;
	TitleUIController*	m_pController	{};	// このキャンバスを管理するコントローラー
	std::list<TitleUI>	m_UIList		{};	// タイトルUIのリスト
	std::string			m_Name			{};	// キャンバスの名前
	bool				m_IsActive		{};	// 有効か？ このCanvasが所有するすべてのUIに影響する。
	bool				m_IsFitScreen{};	// このキャンバスが画面全体にフィットするかどうか。trueなら、画面全体にフィットする。falseなら、指定された範囲内で描画される。
};