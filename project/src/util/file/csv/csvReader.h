#pragma once
#include "../Library/myDxLib.h"
#include <string>
#include <vector>

/// <summary>
/// CSVファイルを読み込むクラス
/// </summary>
class CsvReader {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	CsvReader();
	CsvReader(std::string _filename);
	~CsvReader();

	//================================================================================
	// ▼各種関数

	/// <summary> 
	/// CSVファイルを読み込む
	/// </summary>
	/// <param name="_filename">ファイルパス</param>
	void Load(std::string _filename);

	/// <summary>
	/// 所持データを初期化する
	/// </summary>
	void Reset();

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 文字を取得する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の文字列</returns>
	std::string GetString(int column, int row);

	/// <summary>
	/// 文字を取得する
	/// </summary>
	/// <param name="type">列に書かれた文字</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の文字列</returns>
	std::string GetString(std::string type, int row);

	/// <summary>
	/// 整数を取得する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の整数</returns>
	int GetInt(int column, int row);

	/// <summary>
	/// 整数を取得する
	/// </summary>
	/// <param name="type">列に書かれた文字</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の整数</returns>
	int GetInt(std::string type, int row);

	/// <summary>
	/// 小数を取得する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の少数</returns>
	float GetFloat(int column, int row);

	/// <summary>
	/// 小数を取得する
	/// </summary>
	/// <param name="type">列に書かれた文字</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の少数</returns>
	float GetFloat(std::string type, int row);

	/// <summary>
	/// 可否を取得する
	/// </summary>
	/// <param name="column">列</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の可否</returns>
	bool GetBool(int column, int row);

	/// <summary>
	/// 可否を取得する
	/// </summary>
	/// <param name="type">列に書かれた文字</param>
	/// <param name="row">行</param>
	/// <returns>指定した行列の可否</returns>
	bool GetBool(std::string type, int row);

	// 列数を取得する
	int Column() const { return static_cast<int>(records.size()); }

	// 行数を取得する
	int Row() const { return static_cast<int>(records[max(Column() - 1, 0)].size()); }

	bool CheckSize(int colume, int row) {

		if (records.size() <= colume)
			return false;
		if (records[colume].size() <= row)
			return false;

		return true;
	}

	std::vector<std::vector<std::string>> Records() const { return records; }

private:
	std::vector<std::vector<std::string>> records;	// CSVファイルに書かれた文字を保持する
	std::string filename;	// CSVのファイルパス
};