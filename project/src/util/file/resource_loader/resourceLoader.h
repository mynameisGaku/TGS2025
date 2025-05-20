#pragma once
#include <string>

/// <summary>
/// 画像、サウンド、モデル等のデータを読み込む
/// 既に読み込んでいる物が、再度読み込まれた場合、既にある物のコピーを渡す
/// </summary>
namespace ResourceLoader {

	void Init();

	//==========================================================================================
	// ▼画像関連

	int LoadGraph(std::string filename);
	void DeleteGraph(int handle);

	//==========================================================================================
	// ▼サウンド関連

	int LoadSoundMem(std::string filename);
	void DeleteSoundMem(int handle);

	//==========================================================================================
	// ▼モデル関連

	int MV1LoadModel(std::string filename);
	void MV1DeleteModel(int handle);

	//==========================================================================================
	// ▼各種関数

	void LoadFolderFile(std::string folder, bool loadSubFolder = false);

	/// <summary>
	/// 非同期読み込みをするかを設定する
	/// </summary>
	/// <param name="async">非同期読み込みする場合true</param>
	void SetAsync(bool async);

	/// <summary>
	/// 非同期でファイルを読んでいるか
	/// </summary>
	/// <returns>ファイルを読んでいる途中はtrue</returns>
	bool IsLoading();

	/// <summary>
	/// 読んだファイルを全部消す
	/// </summary>
	void ReleaseAllFile();
};