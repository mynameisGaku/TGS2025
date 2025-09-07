#pragma once
#include "src/util/font/FontDefine.h"

/**
	@brief		フォントの読み込み・管理を行う名前空間
*/
namespace Font {

	//=================================================================================
	//
	//		各種関数
	//
	//=================================================================================

	/**
		@brief		初期化処理
	*/
	void Init();

	/**
		@brief		解放処理
	*/
	void Release();

	//=================================================================================
	//
	//		読み込み処理
	//
	//=================================================================================

	/**
		@brief		フォントを読み込む
		@param		fileHierarchy	:	ファイル階層
		@param		dataName		:	データ名
		@param		fontName		:	フォント名
	*/
	void Load(const std::string& fileHierarchy, const std::string& dataName, const std::string& fontname);

	//=================================================================================
	//
	//		セッター
	//
	//=================================================================================

	/**
		@brief		基本フォントを設定する
		@param		fileHierarchy	:	ファイル階層
		@param		dataName		:	データ名
		@param		fontName		:	フォント名
	*/
	void SetBasicFont(const std::string& fileHierarchy, const std::string& dataName, const std::string& fontname);

	//=================================================================================
	//
	//		ゲッター
	//
	//=================================================================================

	/**
		@brief		フォント情報とタグを指定して、新しいフォントデータを作成します。
					タグが一致している場合は、既存のフォントを再利用します。
		@param		info	:	フォントの情報
		@param		tag		:	フォントに関連付けるタグ
		@returns	フォントのハンドル
	*/
	const int Create(const FontInfo& info, const std::string& tag);

	/**
		@brief		基本フォント情報を取得する。始めに読み込んだフォントが自動で基本フォントとなる。
	*/
	const FontInfo BasicFont();
}