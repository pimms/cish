#pragma once

namespace cish::parse
{

struct CodeMarker {
    bool operator==(const CodeMarker&) const = default;
    int line;
    int col;
    int charOffset;
};
struct CodeInterval {
    bool operator==(const CodeInterval&) const = default;
    CodeMarker start;
    CodeMarker end;
};

}