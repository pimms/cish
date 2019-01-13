#include "StdoutStream.h"

#include <iostream>


namespace cish
{
namespace vm
{


void StdoutStream::write(const std::string &str)
{
    printf("%s", str.c_str());
}


}
}

