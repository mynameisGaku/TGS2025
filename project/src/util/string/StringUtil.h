#pragma once
#include <string>
#include <stdarg.h>
#include <Windows.h>

class StringUtil
{
public:

	/// <summary>
	/// 一文字分保存する
	/// </summary>
	struct Letter {
		std::string str;	// 文字
		float x;	// 描画座標(X軸)
		float y;	// 描画座標(Y軸)

		Letter() : Letter("", 0.0f, 0.0f) {}

		Letter(const std::string& s, float _x, float _y) :
			str(s),
			x(_x),
			y(_y)
		{
		};
	};

	/// <summary>
	/// フォーマットされた文字列をstd::string型に変換する
	/// </summary>
	static inline std::string FormatToString(const char* format, ...) {

		char buffer[1024];  // バッファサイズは必要に応じて調整

		va_list valist;
		va_start(valist, format);
		vsprintf_s(buffer, format, valist);  // バッファにフォーマットされた文字列を書き込む
		va_end(valist);

		return static_cast<std::string>(buffer);
	}

	static std::string UTF8ToShiftJis(std::string data)
	{
		wchar_t str2[512];
		char sjis[512];

		//UTF-8からShift-JISに変換
		MultiByteToWideChar(CP_UTF8, 0, data.c_str(), strlen(data.c_str()) + size_t(1), str2, MAX_PATH);
		WideCharToMultiByte(CP_ACP, 0, str2, sizeof(str2) / sizeof(str2[0]), sjis, sizeof(sjis), NULL, NULL);

		return sjis;

	}
};