#pragma once

#include <string>


namespace cish::vm
{

class IStream
{
public:
    virtual ~IStream() = default;
    virtual void write(const std::string &str) = 0;
};

}
