#include "utils.h"
using namespace utils;

std::string_view utils::nextField(std::string_view &line)
{
    size_t pos = line.find(',');
    std::string_view field = line.substr(0, pos);
    if (pos == std::string_view::npos)
        line = {};
    else
        line.remove_prefix(pos + 1);
    return field;
}