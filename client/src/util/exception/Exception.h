#pragma once
#include <string>

/// <summary>
/// ��O�n�܂Ƃ�
/// </summary>
/// <author>���{��</author>
namespace Exception
{
    /// <summary>
    /// ��O���X���[����
    /// </summary>
    void Throw(const char* fmt, ...);
    void Throw(const std::string& str);
}