#pragma once
#include <DxLib.h>
#include <string>

/**
	@brief	フォントに関する様々な定義を行う名前空間
*/
namespace FontDefine {

	/**
		@brief		フォントの情報を管理する構造体
		@param		handle			:	フォントのハンドル
		@param		size			:	大きさ
		@param		thick			:	太さ
		@param		fontType		:	フォントのタイプ(DX_FONTTYPE_～～)
		@param		charSet			:	文字セット
		@param		edgeSize		:	縁の大きさ
		@param		italic			:	斜め
		@param		color			:	色
		@param		fileHierarchy	:	ファイル階層
		@param		dataName		:	データ名
		@param		fontName		:	フォント名
	*/
	struct FontInfo {
	public:
		FontInfo() :
			handle(-1),
			size(-1),
			thick(-1),
			fontType(DX_FONTTYPE_NORMAL),
			charSet(DX_CHARSET_DEFAULT),
			edgeSize(-1),
			italic(false),
			color(DxLib::GetColor(0, 0, 0)),
			fileHierarchy(),
			dataName(),
			fontName()
		{
		}

		~FontInfo() {

			if (handle != -1)
				DxLib::DeleteFontToHandle(handle);
			handle = -1;

			fileHierarchy.clear();
			dataName.clear();
			fontName.clear();
		}

		//=================================================================================
		//
		//		セッター
		//
		//=================================================================================

		/**
			@brief		ファイル階層、データ名、フォント名を設定する
			@param		fileHierarchy	:	ファイル階層
			@param		dataName		:	データ名
			@param		fontName		:	フォント名
		*/
		inline void SetFilePath(const std::string& fileHierarchy, const std::string& dataName, const std::string& fontName) { SetFileHierarchy(fileHierarchy).SetResourceName(dataName).SetFontName(fontName); }

		/**
			@brief		ファイル階層を設定する
			@param		fileHierarchy	:	ファイル階層
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetFileHierarchy(const std::string& fileHierarchy) { this->fileHierarchy = fileHierarchy; return *this; }

		/**
			@brief		データ名を設定する
			@param		dataName	:	データ名
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetResourceName(const std::string& dataName) { this->dataName = dataName; return *this; }

		/**
			@brief		フォント名を設定する
			@param		fontName	:	フォント名
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetFontName(const std::string& fontName) { this->fontName = fontName; return *this; }

		/**
			@brief		フォントのハンドルを設定する
			@param		handle	:	フォントのハンドル
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetHandle(int handle) { this->handle = handle; return *this; }

		/**
			@brief		フォントの大きさを設定する
			@param		size	:	フォントの大きさ
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetSize(int size) { this->size = size; return *this; }

		/**
			@brief		フォントの太さを設定する
			@param		thick	:	フォントの太さ
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetThick(int thick) { this->thick = thick; return *this; }

		/**
			@brief		フォントのタイプを設定する
			@param		fontType	:	フォントのタイプ(DX_FONTTYPE_～～)
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetFontType(int fontType) { this->fontType = fontType; return *this; }

		/**
			@brief		文字セットを設定する
			@param		charSet		:	文字セット
			@returns	メソッドチェーン

			@details	文字セットについては、DXライブラリのドキュメントを参照してください。
						「CreateFontToHandleについて質問」	:	https://dxlib.xsrv.jp/cgi/patiobbs/patio.cgi?mode=view&no=4155
		*/
		inline FontInfo& SetCharSet(int charSet) { this->charSet = charSet; return *this; }

		/**
			@brief		縁の大きさを設定する
			@param		edgeSize	:	縁の大きさ
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetEdgeSize(int edgeSize) { this->edgeSize = edgeSize; return *this; }

		/**
			@brief		斜め文字の設定を行う
			@param		italic		:	tureなら斜め、falseなら通常
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetItalic(bool italic) { this->italic = italic; return *this; }

		/**
			@brief		色を設定する
			@param		color		:	色
			@returns	メソッドチェーン
		*/
		inline FontInfo& SetColor(int color) { this->color = color; return *this; }

		//=================================================================================
		//
		//		ゲッター
		//
		//=================================================================================

		/**
			@brief		ファイル階層を取得する
		*/
		inline std::string GetFileHierarchy() const { return fileHierarchy; }

		/**
			@brief		データ名を取得する
		*/
		inline std::string GetDataName() const { return dataName; }

		/**
			@brief		フォント名を取得する
		*/
		inline std::string GetFontName() const { return fontName; }

		/**
			@brief		ハンドル値を取得する
		*/
		inline int GetHandle() const { return handle; }

		/**
			@brief		フォントの大きさを取得する
		*/
		inline int GetSize() const { return size; }

		/**
			@brief		フォントの太さを取得する
		*/
		inline int GetThick() const { return thick; }

		/**
			@brief		フォントのタイプを取得する
		*/
		inline int GetFontType() const { return fontType; }

		/**
			@brief		文字セットを取得する
		*/
		inline int GetCharSet() const { return charSet; }

		/**
			@brief		縁の大きさを取得する
		*/
		inline int GetEdgeSize() const { return edgeSize; }

		/**
			@brief		斜めの設定を取得する
		*/
		inline bool GetItalic() const { return italic; }

		/**
			@brief		色を取得する
		*/
		inline int GetColor() const { return color; }

		/**
			@brief		ファイル階層、データ名、フォント名がすべて設定されているかどうかを判定する
		*/
		inline bool CheckName() const { return not fileHierarchy.empty() && not dataName.empty() && not fontName.empty(); }

	private:
		int handle;					// フォントのハンドル
		int size;					// 大きさ
		int thick;					// 太さ
		int fontType;				// フォントのタイプ(DX_FONTTYPE_～～)
		int charSet;				// 文字セット
		int edgeSize;				// 縁の大きさ
		bool italic;				// 斜め
		int color;					// 色
		std::string fileHierarchy;	// ファイル階層
		std::string dataName;		// データ名
		std::string fontName;		// フォント名
	};
}

using namespace FontDefine;