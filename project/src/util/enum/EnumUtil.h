#pragma once
#include <string>
#include <vendor/magic_enum/magic_enum.hpp>


namespace EnumUtil
{
	/// <summary>
	/// string -> enum
	/// 変換失敗時のデフォルト値を指定できる
	/// </summary>
	template<class C>
	C ConvertFromString(const std::string& codeStr, C defaultValue)
	{
		auto key = magic_enum::enum_cast<C>(codeStr);
		if (key.has_value())
		{
			return key.value();
		}
		return defaultValue;
	}

	template<class C>
	std::string ConvertToString(C value)
	{
		std::string key = magic_enum::enum_name(value);
		if (not key.empty())
		{
			return key;
		}
		return "None";
	}
}