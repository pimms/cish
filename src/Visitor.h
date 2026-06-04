#pragma once

namespace cish
{

// Thanks to https://andreasfertig.com/blog/2023/07/visiting-a-stdvariant-safely/
template<class... Ts> struct Visitor: Ts...
{
    using Ts::operator()...;
    consteval void operator()(auto) const { static_assert(false, "Unsupported type"); }
};
template<class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

}

