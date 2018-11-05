#include "Exception.h"

namespace cish
{


Exception::Exception(std::string file, std::string func, int line, const char *format, ...)
{
    char buffer[4096];

    va_list va;
    va_start(va, format);
    vsprintf_s(buffer, format, va);
    va_end(va);

    std::stringstream ss;
    ss  << "General Exception\n"
        << "File: " << file << "\n"
        << "Func: " << func << "\n"
        << "Line: " << line << "\n"
        << "What: " << buffer << "\n";
    _what = ss.str().c_str();
}

Exception::Exception(): _what("") { }

const char* Exception::what() const noexcept
{
    return _what.c_str();
}


};
