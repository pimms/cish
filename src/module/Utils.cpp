#include "Utils.h"

#include <iostream>
#include <sstream>
#include <iomanip>


namespace cish::module::utils
{

void readString(const vm::MemoryView &src, std::vector<char>& out)
{
    uint32_t offset = 0;
    char ch;

    do {
        ch = src.read<char>(offset++);
        out.push_back(ch);
    } while (ch);
}

std::string hexstr(uint32_t num, bool fill)
{
    std::stringstream stream;

    if (fill) {
        stream << std::setfill ('0') << std::setw(sizeof(num)*2);
    }

    stream << std::hex << num;

    std::string res = stream.str();
    return res;
}

}