#include "FopenContext.h"

namespace cish
{
namespace module
{
namespace stdio
{


FopenContext::FopenContext():
    _counter(0x4533345)
{

}

int32_t FopenContext::fopen(const char *path, const char *mode)
{
    FILE *file = ::fopen(path, mode);
    if (!file) {
        return 0;
    }

    const int32_t handle = _counter++;
    _files[handle] = file;
    return handle;
}

int FopenContext::fclose(int32_t handle)
{
    if (_files.count(handle) == 0) {
        return EOF;
    }

    FILE *file = _files[handle];
    const int res = ::fclose(file);
    _files.erase(handle);

    return res;
}

int FopenContext::fgetc(int32_t handle)
{
    if (_files.count(handle) == 0) {
        return EOF;
    }

    FILE *file = _files[handle];
    return ::fgetc(file);
}

bool FopenContext::fgets(std::string *result, int32_t size, uint32_t handle)
{
    if (_files.count(handle) == 0) {
        return false;
    }

    FILE *file = _files[handle];

    std::vector<char> buffer;
    buffer.resize(size);

    if (::fgets(buffer.data(), size, file) == NULL) {
        return false;
    }

    *result = std::string(buffer.data());
    return true;
}

}
}
}
