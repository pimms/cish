#pragma once

#include <memory>
#include <map>

#include "../../vm/Allocation.h"

namespace cish
{
namespace module
{
namespace stdio
{

DECLARE_EXCEPTION(FopenContextException);

class FopenContext
{
public:
    typedef std::shared_ptr<FopenContext> Ptr;

    FopenContext();
    int32_t fopen(const char *path, const  char *mode);
    int fgetc(int32_t handle);

    int fclose(int32_t handle);

private:
    int32_t _counter;
    std::map<int32_t, FILE*> _files;
};


}
}
}
