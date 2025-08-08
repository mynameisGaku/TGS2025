#pragma once
#include <DxLib.h>
#include <windows.h>
#include <list>
#include <functional>
#include <string>
#include <vendor/nlohmann/json.hpp>

/// <summary>
/// イベントがトリガーするタイミング
/// </summary>
enum TUI_EVENT_TRIGGER_TIMING
{
	TUI_EVENT_TRIGGER_TIMING_HOLD,			// 押しっぱでトリガーする
	TUI_EVENT_TRIGGER_TIMING_ENTER,			// 押した瞬間だけトリガーする
	TUI_EVENT_TRIGGER_TIMING_RELEASE,		// 離したときにトリガーする
	TUI_EVENT_TRIGGER_TIMING_HOVER,			// カーソルがUIの上にあるときにトリガーする
	TUI_EVENT_TRIGGER_TIMING_EVERY_FRAME,	// 毎フレーム処理されます
};

/// <summary>
/// 当たり判定方法
/// </summary>
enum TUI_COLLISION_MODE
{
	TUI_COLLISION_MODE_NONE,		// 当たり判定なし
	TUI_COLLISION_MODE_CIRCLE,	// 円の当たり判定
	TUI_COLLISION_MODE_RECT,		// 四角形の当たり判定
};

enum TUI_CANVAS_ANCHOR_POINT
{
	TUI_CANVAS_ANCHOR_POINT_CENTER = 0,
	TUI_CANVAS_ANCHOR_POINT_LEFT,
	TUI_CANVAS_ANCHOR_POINT_RIGHT,
	TUI_CANVAS_ANCHOR_POINT_TOP,
	TUI_CANVAS_ANCHOR_POINT_TOP_LEFT,
	TUI_CANVAS_ANCHOR_POINT_TOP_RIGHT,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM_LEFT,
	TUI_CANVAS_ANCHOR_POINT_BOTTOM_RIGHT,
};

/// <summary>
/// イベント一つのまとまり
/// </summary>
struct TUI_EVENT
{
	TUI_EVENT_TRIGGER_TIMING	Timing					{};	// どのタイミングでトリガーするのか
	std::string					Event					{};	// イベントの名前。文字列で指定。TitleUIControllerに登録されているマップと照合される。
	nlohmann::json				Argument				{}; // 引数
	float						Counter					{};	// 時間を計測するためのカウンター
	float						Duration				{};	// このイベントの効果時間 CounterがDurationを超えた = イベント終了
	std::string					Description				{};	// このイベントの記述。ほぼデバッグ用
};

/// <summary>
/// タイトルUIの記述
/// </summary>
struct UI_TITLE_DESC
{
	std::string				GRAPH_PATH					{};	// 画像のパス
	int						OFFSET_X_FROM_ANCHOR		{};	// アンカーポイントからのオフセット x
	int						OFFSET_Y_FROM_ANCHOR		{};	// アンカーポイントからのオフセット y
	int						GRAPH_SRC_X					{};	// 画像切り抜き開始位置 x
	int						GRAPH_SRC_Y					{};	// 画像切り抜き開始位置 y
	int						GRAPH_DEST_X				{};	// 画像切り抜き終了位置 x
	int						GRAPH_DEST_Y				{};	// 画像切り抜き終了位置 y
	int						INDEX_X						{};	// Canvas上で動かすCursor用のIndex x
	int						INDEX_Y						{};	// Canvas上で動かすCursor用のIndex y
	bool					IS_SELECTABLE				{};	// このUIが選択可能か？ trueなら、カーソルがこのUIの上にあるときに、カーソルが動く。falseなら、カーソルは動かない。
	std::list<TUI_EVENT>	EVENTS						{};	// このUIをクリックしたときに起こるインベントリスト。追加された順に処理されます。
	std::string				Description					{};	// このUIに関する記述。概要とか。デバッグ用
	TUI_COLLISION_MODE		COLLISION					{};	// このUIの当たり判定方法
	std::string				NAME						{}; // このUIの名前
	TUI_CANVAS_ANCHOR_POINT ANCHOR						{};	// このUIのアンカーポイント。Canvasのどこに配置するかを決める。Canvasの座標系に依存する。
};


// 前方宣言
class TitleUICanvas;

/// <summary>
/// タイトルUI
/// </summary>
/// <author> 藤本 </author>
class TitleUI
{
public:
	~TitleUI(){}

	void	Update();
	void	Draw();
	void	Release();

	///		<summary>
	///		記述を元に初期化する
	///		</summary>
	void	Init(const UI_TITLE_DESC& desc, TitleUICanvas* pCanvas);

	///		<summary>
	///		このUIを押しっぱなしにしてるときに呼ばれる
	///		</summary>
	void	OnPressHold();

	///		<summary>
	///		このUIを押した瞬間に呼ばれる
	///		</summary>
	void	OnPressEnter();

	///		<summary>
	///		このUIを離したときに呼ばれる
	///		</summary>
	void	OnPressRelease();

	/// <summary>
	/// 	カーソルがこのUIの上にあるときに呼ばれる
	/// </summary>
	void	OnHover();
	
	///		<summary>
	///		毎フレーム処理されます
	///		</summary>
	void	OnFrame();

private:

	///		<summary>
	///		カーソルとこのUIの当たり判定をチェックします。
	///		</summary>
	void checkCursorCollision();

	int								m_GraphHandle			{};	// 画像のパス すでにロード済みの画像を指定する場合は不要
	int								m_OffsetX_FromAnchor	{};	// アンカーポイントからのオフセット x
	int								m_OffsetY_FromAnchor	{};	// アンカーポイントからのオフセット y
	int								m_GraphSrcX				{};	// 画像切り抜き開始位置 x
	int								m_GraphSrcY				{};	// 画像切り抜き開始位置 y
	int								m_GraphDestX			{};	// 画像切り抜き終了位置 x
	int								m_GraphDestY			{};	// 画像切り抜き終了位置 y
	int								m_IndexX				{};	// Canvas上でのIndex x	重複不可（重複した場合、意図しない動作を引き起こします。）
	int								m_IndexY				{};	// Canvas上でのIndex y	重複不可（重複した場合、意図しない動作を引き起こします。）
	bool							m_IsActive				{};	// 有効か？
	bool							m_IsVisible				{};	// 表示されているか？
	bool							m_IsHolding				{};	// 押下されているか？
	bool							m_IsHover				{};	// カーソルがこのUIの上にあるか？
	bool							m_IsSelectable			{};	// このUIが選択可能か？ trueなら、カーソルがこのUIの上にあるときに、カーソルが動く。falseなら、カーソルは動かない。
	bool							m_IsEventTriggered		{}; // イベントを発動済みか？
	std::string						m_Name					{};	// このUIの名前
	std::string						m_Description			{};	// このUIに関する記述
	std::list<TUI_EVENT>			m_PressHoldEvents		{};	// このUIを長押ししたときに起こるインベントリスト。追加された順に処理されます。
	std::list<TUI_EVENT>			m_PressEnterEvents		{};	// このUIを押した瞬間に起こるインベントリスト。追加された順に処理されます。
	std::list<TUI_EVENT>			m_PressReleaseEvents	{};	// このUIからクリックを離したときに起こるインベントリスト。追加された順に処理されます。
	std::list<TUI_EVENT>			m_HoverEvents			{};	// カーソルがこのUIの上にあるときに起こるイベント。追加された順に処理されます。
	std::list<TUI_EVENT>			m_EveryFrameEvents		{};	// 毎フレーム処理されるイベント。追加された順に処理されます。
	TUI_COLLISION_MODE				m_Collision				{};	// このUIの当たり判定方法。デフォルトはなし。クリックイベントを発火させるために必要。
	TUI_CANVAS_ANCHOR_POINT			m_Anchor				{};	// このUIのアンカーポイント。Canvasのどこに配置するかを決める。Canvasの座標系に依存する。
	TitleUICanvas*					m_pHolder				{};	// このUIを保持しているキャンバス

public:
	// アクセサ
	int								GetGraphHandle()	const { return m_GraphHandle; }
	int								GetOffsetX()		const { return m_OffsetX_FromAnchor; }
	int								GetOffsetY()		const { return m_OffsetY_FromAnchor; }
	int								GetGraphSrcX()		const { return m_GraphSrcX; }
	int								GetGraphSrcY()		const { return m_GraphSrcY; }
	int								GetGraphDestX()		const { return m_GraphDestX; }
	int								GetGraphDestY()		const { return m_GraphDestY; }
	int								GetIndexX()			const { return m_IndexX; }
	int								GetIndexY()			const { return m_IndexY; }
	bool							IsActive()			const { return m_IsActive; }
	bool							IsVisible()			const { return m_IsVisible; }
	bool							IsHolding()			const { return m_IsHolding; }
	bool							IsHover()			const { return m_IsHover; }
	bool							IsSelectable()		const { return m_IsSelectable; }
	std::string						GetDescription()	const { return m_Description; }
	std::string						GetName()			const { return m_Name; }
};