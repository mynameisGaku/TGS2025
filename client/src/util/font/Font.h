#pragma once
#include "framework/myDxLib.h"
#include "src/util/file/FileUtil.h"
#include <string>

class FontInfo;

/// <summary>
/// フォントの読み込み・管理を行う
/// </summary>
namespace Font {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// フォントのデータを読み込む
	/// </summary>
	/// <param name="filePath">フォントが置かれているファイルパス</param>
	/// <param name="resourceName">フォントのリソース名</param>
	/// <param name="fontName">フォントの名前</param>
	void Load(const std::string& filePath, const std::string& resourceName, const std::string& fontName);

	/// <summary>
	/// 指定されたフォント情報からフォントハンドルを生成する
	/// </summary>
	/// <param name="info">フォント情報</param>
	/// <returns>フォントハンドル</returns>
	int CreateFontToHandle(FontInfo* info);

}

/// <summary>
/// フォントの情報
/// </summary>
class FontInfo {
public:
	int handle;		// ハンドル
	int size;		// 大きさ
	int thick;		// 太さ
	int fontType;	// フォントのタイプ(DX_FONTTYPE_~~)
	int charSet;	// 
	int edgeSize;	// 縁の大きさ
	int italic;		// 斜め
	int color;		// 色
	FileUtil::Folder strData;// フォントで使用する文字列情報

	FontInfo() :
		handle(-1),
		size(-1),
		thick(-1),
		fontType(-1),
		charSet(-1),
		edgeSize(-1),
		italic(0),
		color(GetColor(255, 255, 255))
	{
	}

	~FontInfo(){}

	inline void Init(const std::string& tag, const int& _size, const int& _color, const int& _fontType, const int& _edgeSize) {
		strData.tag = tag;
		size = _size;
		color = _color;
		fontType = _fontType;
		edgeSize = _edgeSize;

		Font::CreateFontToHandle(this);
	}
};