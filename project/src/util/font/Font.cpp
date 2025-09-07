#include "Font.h"

#include <unordered_map>

#include "src/util/file/FileUtil.h"
#include "src/util/ptr/PtrUtil.h"

//=================================================================================
//
//		変数
//
//=================================================================================

namespace {

	std::unordered_map<std::string, std::unordered_map<std::string, FontInfo*>>* fonts;	// KEY: データ名, KEY:タグ VALUE: フォント情報
	std::list<FileUtil::Folder>* folderDatas;	// 読み込み済のフォント情報
	FontInfo* bacisFont;						// 基本フォント情報
	bool initialized = false;
}

//=================================================================================
//
//		各種関数
//
//=================================================================================

//---------------------------------------------------------------------------------
void Font::Init() {

	if (fonts == nullptr)
		fonts = new std::unordered_map<std::string, std::unordered_map<std::string, FontInfo*>>();

	if (bacisFont == nullptr) {
		bacisFont = new FontInfo();
		bacisFont->SetHandle(DX_DEFAULT_FONT_HANDLE); // デフォルトのフォントハンドルを設定
	}

	if (folderDatas == nullptr)
		folderDatas = new std::list<FileUtil::Folder>;

	initialized = true;
}

//---------------------------------------------------------------------------------
void Font::Release() {

	if (fonts != nullptr) {
		for (auto font = fonts->begin(); font != fonts->end();) {
			for (auto it = font->second.begin(); it != font->second.end();) {
				delete it->second;
				it = font->second.erase(it);
			}

			font->second.clear();
			font = fonts->erase(font);
		}

		PtrUtil::SafeDelete(fonts);
	}

	PtrUtil::SafeDelete(bacisFont);
	PtrUtil::SafeDelete(folderDatas);

	initialized = false;
}

//=================================================================================
//
//		読み込み処理
//
//=================================================================================

//---------------------------------------------------------------------------------
void Font::Load(const std::string& fileHierarchy, const std::string& dataName, const std::string& fontname) {

	if (not initialized)
		Init();

	bool isRegister = false;	// 既に登録済みか
	for (const auto& filepath : *folderDatas) {
		if (filepath.IsName(dataName) || filepath.IsTag(fontname)) {
			isRegister = true;
			break;
		}
	}

	if (not isRegister) {
		folderDatas->push_back(FileUtil::Folder(fileHierarchy, dataName, fontname));

		// ダングリングポインターを防止  
		const std::string font_path = fileHierarchy + dataName;
		AddFontResourceEx(font_path.c_str(), FR_PRIVATE, NULL);

		OutputDebugString(("Loading Success!	" + font_path + "\n").c_str());
	}

	// 最初に読み込まれたフォント情報を基本フォントとして設定  
	if (bacisFont->GetFontName().empty())
		SetBasicFont(fileHierarchy, dataName, fontname);
}

//=================================================================================
//
//		セッター
//
//=================================================================================

//---------------------------------------------------------------------------------
void Font::SetBasicFont(const std::string& fileHierarchy, const std::string& dataName, const std::string& fontname) {

	if (not initialized)
		Init();

	bacisFont->SetFilePath(fileHierarchy, dataName, fontname);
	bacisFont->SetSize(32).SetColor(GetColor(255, 255, 255)).SetFontType(DX_FONTTYPE_NORMAL);
}

//=================================================================================
//
//		ゲッター
//
//=================================================================================

//---------------------------------------------------------------------------------
const int Font::Create(const FontInfo& info, const std::string& tag) {

	if (not initialized)
		Init();

	const std::string dataName = info.GetDataName();

	if (fonts != nullptr) {
		// 既にフォントデータが存在している場合、それを返す
		if (fonts->contains(dataName) && (*fonts)[dataName].contains(tag))
			return (*fonts)[dataName][tag]->GetHandle();
	}

	FontInfo* fontInfo = new FontInfo(info);

	// データが空っぽなのか確認
	if (fontInfo->GetHandle() < 0 && fontInfo->CheckName()) {

		// 読み込み
		int fontHandle = DxLib::CreateFontToHandle(fontInfo->GetFontName().c_str(), fontInfo->GetSize(), fontInfo->GetThick(), fontInfo->GetFontType(), fontInfo->GetCharSet(), fontInfo->GetEdgeSize(), fontInfo->GetItalic());

		// フォントのハンドルの設定
		if (fontHandle != -1)
			fontInfo->SetHandle(fontHandle);
		// フォントの取得に失敗した場合はデフォルトフォントを設定
		else
			fontInfo->SetHandle(DX_DEFAULT_FONT_HANDLE);
	}

	(*fonts)[dataName][tag] = fontInfo;

	return fontInfo->GetHandle();
}

//---------------------------------------------------------------------------------
const FontInfo Font::BasicFont() {

	if (not initialized)
		Init();

	return *bacisFont;
}
