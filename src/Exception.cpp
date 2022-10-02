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
    _type = "Exception";
    _what = ss.str().c_str();
    _userMessage = buffer;

    __DBGPRINT_EXCEPTION("Exception");
}

Exception::Exception(): _what("") { }
Exception::Exception(const Exception &o):
    _type(o._type),
    _what(o._what),
    _userMessage(o._userMessage)
{ }

Exception& Exception::operator=(const Exception &o)
{
    _what = o._what;
    return *this;
}

const char* Exception::what() const noexcept
{
    return _what.c_str();
}

const char* Exception::userMessage() const noexcept
{
    return _userMessage.c_str();
}

const char* Exception::type() const noexcept
{
    return _type.c_str();
}

};
