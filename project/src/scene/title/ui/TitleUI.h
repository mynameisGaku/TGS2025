#pragma once
#include <DxLib.h>
#include <windows.h>
#include <list>
#include <functional>

/// <summary>
/// イベントがトリガーするタイミング
/// </summary>
enum TUI_EVENT_TRIGGER_TIMING
{
	TUI_EVENT_TRIGGER_TIMING_HOLD,		// 押しっぱでトリガーする
	TUI_EVENT_TRIGGER_TIMING_ENTER,		// 押した瞬間だけトリガーする
	TUI_EVENT_TRIGGER_TIMING_RELEASE,	// 離したときにトリガーする
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

/// <summary>
/// イベント一つのまとまり
/// </summary>
typedef struct 
{
	TUI_EVENT_TRIGGER_TIMING	TIMING					{};	// どのタイミングでトリガーするのか
	const		TCHAR*			EVENT					{};	// イベントの名前。文字列で指定。TitleUIControllerに登録されているマップと照合される。
	bool						IS_WAIT_FOR_COMPLETION	{};	// 実行が完了されるまで待つかどうかのフラグ
	const		TCHAR*			DESCRIPTION				{};	// このイベントの記述。ほぼデバッグ用
}TUI_EVENT;

/// <summary>
/// タイトルUIの記述
/// </summary>
typedef struct
{
	int						GRAPH_HANDLE	{};	// 画像のハンドラ
	int						GRAPH_POS_X		{};	// 描画開始位置 x
	int						GRAPH_POS_Y		{};	// 描画開始位置 y
	int						GRAPH_SRC_X		{};	// 画像切り抜き開始位置 x
	int						GRAPH_SRC_Y		{};	// 画像切り抜き開始位置 y
	int						GRAPH_DEST_X	{};	// 画像切り抜き終了位置 x
	int						GRAPH_DEST_Y	{};	// 画像切り抜き終了位置 y
	int						INDEX_X			{};	// Canvas上で動かすCursor用のIndex x
	int						INDEX_Y			{};	// Canvas上で動かすCursor用のIndex y
	std::list<TUI_EVENT>	EVENTS			{};	// このUIをクリックしたときに起こるインベントリスト。追加された順に処理されます。
	TUI_COLLISION_MODE			COLLISION		{};	// このUIの当たり判定方法
	const		TCHAR*		DESCRIPTION		{};	// このUIに関する記述。概要とか。デバッグ用
}UI_TITLE_DESC;


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
private:
	int								m_GraphHandle			{};	// 画像のパス すでにロード済みの画像を指定する場合は不要
	int								m_GraphPosX				{};	// 描画開始位置 x
	int								m_GraphPosY				{};	// 描画開始位置 y
	int								m_GraphSrcX				{};	// 画像切り抜き開始位置 x
	int								m_GraphSrcY				{};	// 画像切り抜き開始位置 y
	int								m_GraphDestX			{};	// 画像切り抜き終了位置 x
	int								m_GraphDestY			{};	// 画像切り抜き終了位置 y
	bool							m_IsActive				{};	// 有効か？
	bool							m_IsVisible				{};	// 表示されているか？
	bool							m_IsHolding				{};	// 押下されているか？
	bool							m_IsHover				{};	// カーソルがこのUIの上にあるか？
	const TCHAR*					m_Description			{};	// このUIに関する記述
	std::list<TUI_EVENT>			m_PressHoldEvents		{};	// このUIを長押ししたときに起こるインベントリスト。追加された順に処理されます。
	std::list<TUI_EVENT>			m_PressEnterEvents		{};	// このUIを押した瞬間に起こるインベントリスト。追加された順に処理されます。
	std::list<TUI_EVENT>			m_PressReleaseEvents	{};	// このUIからクリックを離したときに起こるインベントリスト。追加された順に処理されます。
	TUI_COLLISION_MODE					m_Collision				{};	// このUIの当たり判定方法。デフォルトはなし。クリックイベントを発火させるために必要。
	TitleUICanvas*					m_pHolder				{};	// このUIを保持しているキャンバス
};