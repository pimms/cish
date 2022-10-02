#pragma once

#include <stdexcept>
#include <sstream>
#include <stdarg.h>

#if defined(__APPLE__) || defined(__GNUC__)
    #define vsprintf_s vsprintf
#endif


#ifdef DEBUG
    #define __DBGPRINT_EXCEPTION(_name) \
        fprintf(stderr, "throwing %s at %s:%d (%s): %s\n", \
                _name, file.c_str(), line, func.c_str(), buffer);
#else
    #define __DBGPRINT_EXCEPTION(_name)
#endif

#define DECLARE_EXCEPTION(name)                     \
    class name : public ::cish::Exception           \
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
            _userMessage = buffer;                  \
            _type = #name;                          \
            __DBGPRINT_EXCEPTION(#name)             \
        }                                           \
                                                    \
        void raise() override { throw *this; }      \
    };

#define Throw(_TYPE, ...) \
    { \
        _TYPE __e__(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); __e__.raise(); \
        /* SHOULD NEVER HAPPEN --> */ assert(0); throw __e__;\
    }

namespace cish
{

class Exception
{
public:
    Exception(std::string file, std::string func, int line, const char *format, ...);
    Exception();

    Exception(const Exception &o);
    Exception& operator=(const Exception &o);

    virtual ~Exception() = default;
    virtual const char* what() const noexcept;
    virtual const char* userMessage() const noexcept;
    virtual const char* type() const noexcept;
    virtual void raise() { throw *this; }

protected:
    std::string _type;
    std::string _what;
    std::string _userMessage;
};

}
