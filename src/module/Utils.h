#pragma once

#include "../vm/Allocation.h"

#include <vector>
#include <string>


namespace cish::module::utils
{

/**
 * Reads a strings from the MemoryView into the output-vector.
 * This function assumes that the string pointer to by 'src'
 * is null-terminated.
 */
void readString(const vm::MemoryView &src, std::vector<char>& out);

std::string hexstr(uint32_t num, bool fill);

}