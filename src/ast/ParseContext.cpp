#include "ParseContext.h"
#include "AntlrContext.h"


namespace cish
{
namespace ast
{


ParseContext::Ptr ParseContext::parseSource(const std::string &source)
{
    return std::make_shared<AntlrContext>(source);
}


}
}