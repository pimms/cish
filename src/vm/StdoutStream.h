#pragma once

#include "IStream.h"

namespace cish::vm
{

class StdoutStream : public IStream
{
public:
    void write(const std::string &str) override;
};

}
