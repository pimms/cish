#pragma once

#include <exception>
#include <sstream>
#include <cassert>
#include <cstdarg>

#ifdef DEBUG
    #define DBGPRINT_EXCEPTION(_name) \
        fprintf(stderr, "throwing %s at %s:%d (%s): %s\n", \
                _name, file.c_str(), line, func.c_str(), buffer);
#else
    #define DBGPRINT_EXCEPTION(_name)
#endif

#define DECLARE_EXCEPTION(name)                     \
    class name : public ::cish::Exception           \
    {                                               \
    public:                                         \
        name(const std::string& file,               \
             const std::string& func,               \
             int line,                              \
             const char *format, ...)               \
        {                                           \
            char buffer[4096];                      \
                                                    \
            va_list va;                             \
            va_start(va, format);                   \
            vsnprintf(buffer, 4096, format, va);    \
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
            DBGPRINT_EXCEPTION(#name)               \
        }                                           \
    };

#define Throw(_TYPE, ...) \
    { \
        throw _TYPE(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
        /* SHOULD NEVER HAPPEN --> */ assert(0);                    \
        /* SHOULD ABSOLUTELY NEVER HAPPEN --> */ exit(1);           \
    }

namespace cish
{

class Exception: public std::exception
{
public:
    Exception(std::string file, std::string func, int line, const char *format, ...);
    Exception();

    Exception(const Exception &o) noexcept;
    Exception& operator=(const Exception &o) noexcept;

    ~Exception() override = default;
    const char* what() const noexcept override;
    virtual const char* userMessage() const noexcept;
    virtual const char* type() const noexcept;

protected:
    std::string _type;
    std::string _what;
    std::string _userMessage;
};

}
