#pragma once

#include <stdexcept>
#include <sstream>
#include <stdarg.h>



#if defined(__APPLE__) || defined(__GNUC__)
    #define vsprintf_s vsprintf
#endif




#ifdef DEBUG
    #define __DBGPRINT_EXCEPTION(_name) \
        printf("throwing %s at %s:%d (%s): %s\n", \
                _name, file.c_str(), line, func.c_str(), buffer);
#else
    #define __DBGPRINT_EXCEPTION(_name)
#endif


#define DECLARE_EXCEPTION(name)                     \
    class name : ::cish::Exception                   \
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
            __DBGPRINT_EXCEPTION(#name)             \
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

#define Throw(_TYPE, ...) \
    throw _TYPE(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

namespace cish
{

class Exception: ::std::exception
{
public:
    Exception(std::string file, std::string func, int line, const char *format, ...);
    Exception();
    virtual const char* what() const noexcept override;

private:
    std::string _what;
};

}
