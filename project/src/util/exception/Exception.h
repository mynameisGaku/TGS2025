#pragma once
#include <string>

/// <summary>
/// 例外系まとめ
/// </summary>
/// <author>藤本樂</author>
namespace Exception
{
    /// <summary>
    /// 例外をスローする
    /// </summary>
    void Throw(const char* fmt, ...);
    void Throw(const std::string& str);
}