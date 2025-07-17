#pragma once
#include <string>
#include <vendor/magic_enum/magic_enum.hpp>
#include <type_traits>


namespace EnumUtil
{
	/// <summary>
	/// string -> enum
	/// �ϊ����s���̃f�t�H���g�l���w��ł���
	/// </summary>
	template<typename C>
	C ToEnum(const std::string& codeStr, C defaultValue)
	{
		auto key = magic_enum::enum_cast<C>(codeStr);
		if (key.has_value())
		{
			return key.value();
		}
		return defaultValue;
	}

	template<typename C>
	std::enable_if_t<std::is_enum_v<C>, std::string_view>
		ToStringView(C value)
	{
		return magic_enum::enum_name(value);
	}

	template<typename C>
	std::enable_if_t<std::is_enum_v<C>, std::string>
		ToString(C value)
	{
		auto strv = ToStringView(value);
		return std::string(strv.begin(), strv.end());
	}
}