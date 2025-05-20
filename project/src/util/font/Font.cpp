#include "src/util/font/Font.h"

#include <vector>
#include <unordered_map>

namespace {

	std::unordered_map<std::string, std::unordered_map<int, FontInfo>>* fonts;	// フォントデータ [フォントの名前][サイズ] フォント情報
	std::list<String::Folder>* folderStrDatas;	// 読み込み済のフォント情報
}

void Font::Init() {

	if (fonts == nullptr)
		fonts = new std::unordered_map<std::string, std::unordered_map<int, FontInfo>>;

	if (folderStrDatas == nullptr)
		folderStrDatas = new std::list<String::Folder>;
}

void Font::Release() {

	if (fonts != nullptr) {

		for (auto type = fonts->begin(); type != fonts->end();) {
			for (auto& font : type->second) {
				DeleteFontToHandle(font.second.handle);
			}

			type->second.clear();
			type = fonts->erase(type);
		}

		Function::DeletePointer(fonts);
	}

	Function::DeletePointer(folderStrDatas);
}

void Font::Load(const std::string& filePath, const std::string& resourceName, const std::string& fontName) {

	//========================================================================
	// ◇フォント読み込み

	LPCSTR font_path = filePath.c_str();
	AddFontResourceEx(font_path, FR_PRIVATE, NULL);

	//========================================================================
	// ◇領域確保

	if (fonts == nullptr)
		fonts = new std::unordered_map<std::string, std::unordered_map<int, FontInfo>>;

	(*fonts)[fontName];

	//========================================================================
	// ◇文字列を格納

	if (folderStrDatas == nullptr)
		folderStrDatas = new std::list<String::Folder>;

	folderStrDatas->push_back(String::Folder(filePath, resourceName, fontName));
}

int Font::CreateFontToHandle(FontInfo* info) {

	if (fonts != nullptr) {
		// 既にフォントデータが存在している場合、それを返す
		if (fonts->contains(info->strData.tag) && (*fonts)[info->strData.tag].contains(info->size)) {
			*info = (*fonts)[info->strData.tag][info->size];
			return info->handle;
		}
	}

	if (folderStrDatas == nullptr)
		return -1;

	for (const auto& itr : *folderStrDatas) {
		// 指定されたフォント名がロードされているか
		if (info->strData.IsTag(itr.tag)) {
			info->handle = DxLib::CreateFontToHandle(info->strData.tag.c_str(), info->size, info->thick, info->fontType, info->charSet, info->edgeSize, info->italic);
			(*fonts)[info->strData.tag][info->size] = *info;
			return info->handle;
		}
	}
	
	return -1;
}
