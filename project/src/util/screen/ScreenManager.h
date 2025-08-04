#pragma once
#include "src/util/math/Vector2.h"
#include <string>

/// <summary>
/// 画面情報
/// </summary>
struct ScreenData {
	ScreenData() :
		handle(-1),
		clipX(0),
		clipY(0),
		drawX(0),
		drawY(0),
		tag("")
	{
		int w, h;
		GetWindowSize(&w, &h);
		clipW = w;
		clipH = h;
		drawW = w;
		drawH = h;
	}

	int handle;	// 画面の画像ハンドル

	int clipX;	// 画面の切り抜き開始地点
	int clipY;	// 画面の切り抜き開始地点
	int clipW;	// 画面の切り抜き幅
	int clipH;	// 画面の切り抜き高さ

	int drawX;	// 描画領域の座標
	int drawY;	// 描画領域の座標
	int drawW;	// 描画領域の幅
	int drawH;	// 描画領域の高さ

	std::string tag;	// タグ
};

namespace ScreenManager
{
	void Init();
	void Update();
	void Release();

	int GetScreenDivCount();

	int GetScreenDivWidth();
	int GetScreenDivHeight();

	Vector2 GetScreenBeginPos(int divCount);
	Vector2 GetScreenEndPos(int divCount);

	/// <summary>
	/// 指定されたデータからスクリーンを作成します。
	/// </summary>
	/// <param name="data">スクリーンの作成に使用するScreenData構造体への参照。</param>
	void CreateScreen(const ScreenData& data);

	/// <summary>
	/// 指定されたタグと描画位置・サイズでスクリーンを作成します。
	/// </summary>
	/// <param name="tag">スクリーンを識別するための文字列タグ。</param>
	/// <param name="drawX">スクリーンの描画開始位置（X座標）。</param>
	/// <param name="drawY">スクリーンの描画開始位置（Y座標）。</param>
	/// <param name="drawW">スクリーンの幅（ピクセル単位）。</param>
	/// <param name="drawH">スクリーンの高さ（ピクセル単位）。</param>
	void CreateScreen(const std::string& tag, int drawX, int drawY, int drawW, int drawH);

	/// <summary>
	/// 指定されたタグで描画処理の開始を示します。
	/// </summary>
	/// <param name="tag">描画処理の識別に使用するタグ名。</param>
	void DrawBegin(const std::string& tag);

	/// <summary>
	/// 指定されたタグに対応する描画の終了処理を行います。
	/// </summary>
	/// <param name="tag">終了処理を行う描画タグ名。</param>
	void DrawEnd(const std::string& tag);

	/// <summary>
	/// リソースや状態をクリーンアップします。
	/// </summary>
	void CleanUp();

	/// <summary>
	/// 指定したタグに関連付けられたクリップの位置を設定します。
	/// </summary>
	/// <param name="tag">位置を設定するクリップを識別するためのタグ文字列。</param>
	/// <param name="x">クリップの新しいX座標。</param>
	/// <param name="y">クリップの新しいY座標。</param>
	void SetClipPos(const std::string& tag, int x, int y);

	/// <summary>
	/// 指定されたタグに関連付けられたクリップのサイズを設定します。
	/// </summary>
	/// <param name="tag">サイズを設定するクリップを識別するためのタグ文字列。</param>
	/// <param name="w">クリップの幅（ピクセル単位）。</param>
	/// <param name="h">クリップの高さ（ピクセル単位）。</param>
	void SetClipSize(const std::string& tag, int w, int h);

	/// <summary>
	/// 指定したタグに関連付けられた描画位置を設定します。
	/// </summary>
	/// <param name="tag">描画位置を設定する対象を識別するためのタグ文字列。</param>
	/// <param name="x">新しいX座標値。</param>
	/// <param name="y">新しいY座標値。</param>
	void SetDrawPos(const std::string& tag, int x, int y);

	/// <summary>
	/// 指定されたタグに関連付けられた描画サイズを設定します。
	/// </summary>
	/// <param name="tag">描画サイズを設定する対象を識別するためのタグ文字列。</param>
	/// <param name="w">設定する幅（ピクセル単位）。</param>
	/// <param name="h">設定する高さ（ピクセル単位）。</param>
	void SetDrawSize(const std::string& tag, int w, int h);

	/// <summary>
	/// 指定されたタグに対応するScreenDataオブジェクトを取得します。
	/// </summary>
	/// <param name="tag">取得したいScreenDataに関連付けられたタグ文字列。</param>
	/// <returns>見つかった場合はScreenDataへのポインタ、見つからない場合はnullptrを返します。</returns>
	ScreenData* GetScreenData(const std::string& tag);

	/// <summary>
	/// 指定されたタグを持つスクリーンが存在するかどうかを判定します。
	/// </summary>
	/// <param name="tag">存在を確認するスクリーンのタグ。</param>
	/// <returns>スクリーンが存在する場合は true、存在しない場合は false を返します。</returns>
	bool IsScreenExist(const std::string& tag);
}