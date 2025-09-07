//-----------------------------------------------------------------------------
// File : FileUtil.h
// Desc : File Utility.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <string>
#include <list>
#include <Shlwapi.h>
#include <filesystem>


//-----------------------------------------------------------------------------
// Linker
//-----------------------------------------------------------------------------
#pragma comment( lib, "shlwapi.lib ")

namespace FileUtil
{


//-----------------------------------------------------------------------------
//! @brief      ファイルパスを検索します.
//!
//! @param[in]      filePath        検索するファイスパス.
//! @param[out]     result          検索結果の格納先.
//! @retval true    ファイルを発見.
//! @retval false   ファイルが見つからなかった.
//! @memo 検索ルールは以下の通り.
//!      .\
//!      ..\
//!      ..\..\
//!      .\res\
//!      %EXE_DIR%\
//!      %EXE_DIR%\..\
//!      %EXE_DIR%\..\..\
//!      %EXE_DIR%\res\
//-----------------------------------------------------------------------------
bool SearchFilePathA(const char* filename, std::string& result);

//-----------------------------------------------------------------------------
//! @brief      ファイルパスを検索します.
//!
//! @param[in]      filePath        検索するファイスパス.
//! @param[out]     result          検索結果の格納先.
//! @retval true    ファイルを発見.
//! @retval false   ファイルが見つからなかった.
//! @memo 検索ルールは以下の通り.
//!      .\
//!      ..\
//!      ..\..\
//!      .\res\
//!      %EXE_DIR%\
//!      %EXE_DIR%\..\
//!      %EXE_DIR%\..\..\
//!      %EXE_DIR%\res\
//-----------------------------------------------------------------------------
bool SearchFilePathW(const wchar_t* filename, std::wstring& result);

//-----------------------------------------------------------------------------
//! @brief      ディレクトリパスを削除し，ファイル名を返却します.
//!
//! @param[in]      path        ディスレクトリパスを取り除くファイルパス.
//! @return     ファイル名を返却します.
//-----------------------------------------------------------------------------
std::string RemoveDirectoryPathA(const std::string& path);

//-----------------------------------------------------------------------------
//! @brief      ディレクトリパスを削除し，ファイル名を返却します.
//!
//! @param[in]      path        ディスレクトリパスを取り除くファイルパス.
//! @return     ファイル名を返却します.
//-----------------------------------------------------------------------------
std::wstring RemoveDirectoryPathW(const std::wstring& path);

//-----------------------------------------------------------------------------
//! @brief      ディレクトリ名を取得します.
//!
//! @param[in]      filePath        ファイルパス.
//! @return     指定されたファイルパスからディレクトリ名を抜き出します.
//-----------------------------------------------------------------------------
std::string GetDirectoryPathA(const char* path);

//-----------------------------------------------------------------------------
//! @brief      ディレクトリ名を取得します.
//!
//! @param[in]      filePath        ファイルパス.
//! @return     指定されたファイルパスからディレクトリ名を抜き出します.
//-----------------------------------------------------------------------------
std::wstring GetDirectoryPathW(const wchar_t* path);


#if defined(UNICODE) || defined(_UNICODE)
    inline bool SearchFilePath(const wchar_t* filename, std::wstring& result)
    { return SearchFilePathW(filename, result); }

    inline std::wstring RemoveDirectoryPath(const std::wstring& path)
    { return RemoveDirectoryPathW(path); }

    inline std::wstring GetDirectoryPath(const wchar_t* path)
    { return GetDirectoryPathW(path); }
#else
    inline bool SearchFilePath(const char* filename, std::string& result)
    { return SearchFilePathA(filename, result); }

    inline std::string RemoveDirectoryPath(const std::string& path)
    { return RemoveDirectoryPathA(path); }

    inline std::string GetDirectoryPath(const char* path)
    { return GetDirectoryPathA(path); }
#endif//defined(UNICODE) || defined(_UNICODE)

	/// <summary>
	/// フォルダパスの情報を持つ
	/// </summary>
	struct Folder {
		std::string path;	// ファイルパス
		std::string name;	// リソース名
		std::string tag;	// タグ

		Folder() :
			path(""),
			name(""),
			tag("")
		{
		}

		/// <summary>
		/// フォルダパスの情報を設定する
		/// </summary>
		/// <param name="_path">ファイルパス</param>
		/// <param name="_name">リソース名</param>
		/// <param name="_tag">タグ</param>
		Folder(const std::string& _path, const std::string& _name, const std::string& _tag) :
			path(_path),
			name(_name),
			tag(_tag)
		{
		}

		/// <summary>
		/// 指定されたファイルパスと一致しているか
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>一致している場合true</returns>
		inline bool IsPath(const std::string& path) const { return this->path == path; }

		/// <summary>
		/// 指定されたリソース名と一致しているか
		/// </summary>
		/// <param name="name">リソース名</param>
		/// <returns>一致している場合true</returns>
		inline bool IsName(const std::string& name) const { return this->name == name; }

		/// <summary>
		/// 指定されたタグと一致しているか
		/// </summary>
		/// <param name="tag">タグ</param>
		/// <returns>一致している場合true</returns>
		inline bool IsTag(const std::string& tag) const { return this->tag == tag; }
	};

	// フォルダ内の全ファイルのファイル名を取得
	inline std::list<std::string> FindFileNames(std::string folder, bool removeExtention)
	{
		std::list<std::string> fileNames;

		std::string f;
		for (const std::filesystem::directory_entry& i : std::filesystem::recursive_directory_iterator(folder))
		{
			std::filesystem::path p = i.path().filename();
			f = p.string();

			//拡張子の分を消す
			if (removeExtention)
			{
				size_t sub_s = f.find_last_of(".");
				f = f.substr(0, sub_s);
			}

			fileNames.push_back(f);
		}

		return fileNames;
	}
}

using namespace FileUtil;