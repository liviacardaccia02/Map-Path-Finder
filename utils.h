#ifndef UTILS_H
#define UTILS_H

#include <string_view>

namespace utils
{
    /* Extract the next field from a comma-separated line */
    std::string_view nextField(std::string_view &line);
}

#endif