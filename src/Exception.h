#pragma once

#include <stdexcept>
#include <sstream>
#include <stdarg.h>



#if defined(__APPLE__) || defined(__GNUC__)
    #define vsprintf_s vsprintf
#endif

#define DECLARE_EXCEPTION(name)                     \
    class name : ::std::exception                   \
    {                                               \
    public:                                         \
        name(std::string file, std::string func,    \
             int line, const char *format, ...)     \
        {                                           \
            char buffer[4096];                      \
                                                    \
            va_list va;                             \
            va_start(va, format);                   \
            vsprintf_s(buffer, format, va);         \
            va_end(va);                             \
                                                    \
            std::stringstream ss;                   \
            ss  << #name << "\n"                    \
                << "File: " << file << "\n"         \
                << "Func: " << func << "\n"         \
                << "Line: " << line << "\n"         \
                << "What: " << buffer << "\n";      \
            _what = ss.str().c_str();               \
        }                                           \
                                                    \
        virtual const char* what() const noexcept override \
        {                                           \
            return _what.c_str();                   \
        }                                           \
                                                    \
    private:                                        \
        std::string _what;                          \
    };

#define Throw(_TYPE, ...) { throw _TYPE(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); }

namespace cish
{

DECLARE_EXCEPTION(Exception);

}
