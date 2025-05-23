#pragma once
#include <string>
#include <stdarg.h>

class StringUtil
{
public:

	/// <summary>
	/// �ꕶ�����ۑ�����
	/// </summary>
	struct Letter {
		std::string str;	// ����
		float x;	// �`����W(X��)
		float y;	// �`����W(Y��)

		Letter() : Letter("", 0.0f, 0.0f) {}

		Letter(const std::string& s, float _x, float _y) :
			str(s),
			x(_x),
			y(_y)
		{
		};
	};

	/// <summary>
	/// �t�H�[�}�b�g���ꂽ�������std::string�^�ɕϊ�����
	/// </summary>
	static inline std::string FormatToString(const char* format, ...) {

		char buffer[1024];  // �o�b�t�@�T�C�Y�͕K�v�ɉ����Ē���

		va_list valist;
		va_start(valist, format);
		vsprintf_s(buffer, format, valist);  // �o�b�t�@�Ƀt�H�[�}�b�g���ꂽ���������������
		va_end(valist);

		return static_cast<std::string>(buffer);
	}
};