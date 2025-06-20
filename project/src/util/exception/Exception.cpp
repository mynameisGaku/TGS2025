#include "Exception.h"

#include <Windows.h>
#include <string>
#include <stdexcept>

namespace Exception
{
    void Exception::Throw(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        std::string s(buffer);
        s.insert(0, "[Exception] ");
        throw std::runtime_error("ó·äOÇ™î≠ê∂ÇµÇ‹ÇµÇΩ: " + s);
    }

    void Throw(const std::string& str)
    {
        Throw(str.c_str());
    }

}