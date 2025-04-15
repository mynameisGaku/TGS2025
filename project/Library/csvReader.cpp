#include "csvReader.h"

// ���ėp
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
		// ��s���̕�����
		std::string rows;

		// �s�̒��ɂ��镶������擾
		while (std::getline(ifs, rows)) {

			// �s�̗v�f���J���}��؂�Ő؂����āA�ێ�����
			std::vector<std::string> strs;

			// �؂�����J�n���镶����
			int top = 0;

			for (int i = 0; i <= rows.length(); i++) {
				if (rows[i] == ',') {
					// �ǂݐi�߂�͈�
					int read = i - top;
					if (read < 0)
						read = 0;

					// substr(a, b) = a�����ڂ���Ab�����i�񂾔͈�
					std::string s = rows.substr(top, read);

					strs.push_back(s);
					top = i + 1;
				}
			}

			// �Ō�̗v�f�̓J���}�����m�o���Ȃ��̂ł����ŏE���Ă���
			strs.push_back(rows.substr(top));

			// �����o�ϐ��ɕۑ�
			records.push_back(strs);
		}

		// �t�@�C�������
		ifs.close();
	}
	else {
		// �G���[���b�Z�[�W
		std::string error = _filename + "�̃t�@�C����������܂���";
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
		// �G���[���b�Z�[�W
		std::string error = filename + "�̓ǂݎ�肪�������@�\���܂���ł���";
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
