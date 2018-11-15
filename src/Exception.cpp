#include "Exception.h"

namespace cish
{

static void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	size_t pos = data.find(toSearch);
	while(pos != std::string::npos) {
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + toSearch.size());
	}
}


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

#ifdef DEBUG
    printf("throwing cish::Exception at %s:%d (%s): %s\n",
            file.c_str(), line, func.c_str(), buffer);
#endif
}

Exception::Exception(): _what("") { }
Exception::Exception(const Exception &o): _what(o._what) { }

Exception& Exception::operator=(const Exception &o)
{
    _what = o._what;
    return *this;
}

const char* Exception::what() const noexcept
{
    return _what.c_str();
}


};
