#include "Utils.h"


namespace cish
{
namespace module
{
namespace utils
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



}
}
}