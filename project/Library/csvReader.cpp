#include "csvReader.h"

// ◇汎用
#include <fstream>
#include <assert.h>

CsvReader::CsvReader() : CsvReader("") {}

CsvReader::CsvReader(std::string _filename) {

	if (_filename != "")
		Load(_filename);
}

CsvReader::~CsvReader() {

	Reset();
}

void CsvReader::Load(std::string _filename) {

	filename = _filename;
	std::ifstream ifs(filename);

	if (ifs) {
		// 一行分の文字列
		std::string rows;

		// 行の中にある文字列を取得
		while (std::getline(ifs, rows)) {

			// 行の要素をカンマ区切りで切り取って、保持する
			std::vector<std::string> strs;

			// 切り取りを開始する文字列数
			int top = 0;

			for (int i = 0; i <= rows.length(); i++) {
				if (rows[i] == ',') {
					// 読み進める範囲
					int read = i - top;
					if (read < 0)
						read = 0;

					// substr(a, b) = a文字目から、b文字進んだ範囲
					std::string s = rows.substr(top, read);

					strs.push_back(s);
					top = i + 1;
				}
			}

			// 最後の要素はカンマが検知出来ないのでここで拾ってくる
			strs.push_back(rows.substr(top));

			// メンバ変数に保存
			records.push_back(strs);
		}

		// ファイルを閉じる
		ifs.close();
	}
	else {
		// エラーメッセージ
		std::string error = _filename + "のファイルが見つかりません";
		assert(0 && error.c_str());
	}
}

void CsvReader::Reset() {

	for (auto& itr : records)
		itr.clear();

	records.clear();
}

std::string CsvReader::GetString(int column, int row) {

	bool checkColumn = (column >= 0 && column < records.size());
	bool checkRow = (row >= 0 && row < records[column].size());

	if (!checkColumn || !checkRow) {
		// エラーメッセージ
		std::string error = filename + "の読み取りが正しく機能しませんでした";
		assert(0 && error.c_str());
	}

	return records[column][row];
}

std::string CsvReader::GetString(std::string type, int row) {

	for (int i = 0; i < records.size(); i++) {
		std::vector<std::string> rows = records[i];
		if (rows[0] == type) {
			return GetString(i, row);
		}
	}

	return "";
}

int CsvReader::GetInt(int column, int row) {

	return stoi(GetString(column, row));
}

int CsvReader::GetInt(std::string type, int row) {

	for (int i = 0; i < records.size(); i++) {
		std::vector<std::string> rows = records[i];
		if (rows[0] == type) {
			return stoi(GetString(i, row));
		}
	}

	return -1;
}

float CsvReader::GetFloat(int column, int row) {

	return stof(GetString(column, row));
}

float CsvReader::GetFloat(std::string type, int row) {

	for (int i = 0; i < records.size(); i++) {
		std::vector<std::string> rows = records[i];
		if (rows[0] == type) {
			return stof(GetString(i, row));
		}
	}

	return -1;
}

bool CsvReader::GetBool(int column, int row) {

	std::string letter = GetString(column, row);
	std::vector<std::string> truth = { "true", "True", "TRUE", "1" };

	for (const auto& t : truth) {
		if (letter == t)
			return true;
	}

	return false;
}

bool CsvReader::GetBool(std::string type, int row) {

	for (int i = 0; i < records.size(); i++) {
		std::vector<std::string> rows = records[i];
		if (rows[0] == type)
			return GetBool(i, row);
	}

	return false;
}
