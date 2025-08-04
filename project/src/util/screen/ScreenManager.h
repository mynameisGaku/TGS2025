#pragma once
#include "src/util/math/Vector2.h"
#include <string>

/// <summary>
/// スクリーン画像情報
/// </summary>
struct ScreenImageData {
	ScreenImageData() :
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

	int clipX;	// 画面への切り抜き開始地点
	int clipY;	// 画面への切り抜き開始地点
	int clipW;	// 画面への切り抜き幅
	int clipH;	// 画面への切り抜き高さ

	int drawX;	// 描画領域の座標
	int drawY;	// 描画領域の座標
	int drawW;	// 描画領域の幅
	int drawH;	// 描画領域の高さ

	std::string tag;	// 識別用タグ
};

/// <summary>
/// 画面に映されたものをスクリーン画像として管理して、書き込みや描画処理を行う。
/// </summary>
namespace ScreenManager
{
	void Init();
	void Update();
	void Release();

	/// <summary>
	/// 指定されたデータからスクリーン画像を作成します。
	/// </summary>
	/// <param name="data">スクリーン画像作成に使用するScreenImageData構造体</param>
	void CreateScreen(const ScreenImageData& data);

	/// <summary>
	/// 指定されたタグと描画位置・サイズでスクリーン画像を作成します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <param name="drawX">スクリーン画像の描画開始位置（X座標）</param>
	/// <param name="drawY">スクリーン画像の描画開始位置（Y座標）</param>
	/// <param name="drawW">スクリーン画像の幅（ピクセル単位）</param>
	/// <param name="drawH">スクリーン画像の高さ（ピクセル単位）</param>
	void CreateScreen(const std::string& tag, int drawX, int drawY, int drawW, int drawH);

	/// <summary>
	/// 指定されたタグで描画処理の開始を示します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	void DrawBegin(const std::string& tag);

	/// <summary>
	/// 指定されたタグに対応する描画の終了処理を行います。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	void DrawEnd(const std::string& tag);

	/// <summary>
	/// リソースや状態をクリーンアップします。
	/// </summary>
	void CleanUp();

	/// <summary>
	/// 指定したタグに関連付けられた画面への切り抜きの位置を設定します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <param name="x">画面への切り抜きの新しいX座標</param>
	/// <param name="y">画面への切り抜きの新しいY座標</param>
	void SetClipPos(const std::string& tag, int x, int y);

	/// <summary>
	/// 指定されたタグに関連付けられた画面への切り抜きのサイズを設定します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <param name="w">画面への切り抜きの幅（ピクセル単位）</param>
	/// <param name="h">画面への切り抜きの高さ（ピクセル単位）</param>
	void SetClipSize(const std::string& tag, int w, int h);

	/// <summary>
	/// 指定したタグに関連付けられた描画位置を設定します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <param name="x">新しいX座標値</param>
	/// <param name="y">新しいY座標値</param>
	void SetDrawPos(const std::string& tag, int x, int y);

	/// <summary>
	/// 指定されたタグに関連付けられた描画サイズを設定します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <param name="w">設定する幅（ピクセル単位）</param>
	/// <param name="h">設定する高さ（ピクセル単位）</param>
	void SetDrawSize(const std::string& tag, int w, int h);

	/// <summary>
	/// 指定されたタグに対応するScreenImageDataオブジェクトを取得します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <returns>見つかった場合はScreenImageDataへのポインタ、見つからない場合はnullptrを返します。</returns>
	ScreenImageData* GetScreenImageData(const std::string& tag);

	/// <summary>
	/// 指定されたタグを持つスクリーン画像が存在するかどうかを判定します。
	/// </summary>
	/// <param name="tag">識別用タグ</param>
	/// <returns>スクリーン画像が存在する場合は true、存在しない場合は false を返します。</returns>
	bool IsScreenExist(const std::string& tag);
}