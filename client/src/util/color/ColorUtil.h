#pragma once
#include <string>

namespace ColorUtil
{
	inline int ColorFromString(const std::string& colorName)
	{
		if (colorName == "Red")				return 0xFF0000;
		else if (colorName == "Green")		return 0x00FF00;
		else if (colorName == "Blue")		return 0x0000FF;
		else if (colorName == "Black")		return 0x000000;
		else if (colorName == "White")		return 0xFFFFFF;
		else if (colorName == "Yellow")		return 0xFFFF00;
		else if (colorName == "Cyan")		return 0x00FFFF;
		else if (colorName == "Magenta")	return 0xFF00FF;
		else return -1; // Invalid color
	}
}