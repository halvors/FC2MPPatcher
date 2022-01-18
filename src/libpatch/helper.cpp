#include "helper.h"

#include <sstream>

uint32_t Helper::toInt(const std::string& version)
{
    std::istringstream stream(version);
    std::string s;

    uint32_t array[3] = { 0 };
    size_t index = 0;

    while (std::getline(stream, s, '.'))
        array[index++] = std::stoi(s);

    uint32_t major = (array[0] << 24) & 0xff000000;
    uint32_t minor = (array[1] << 16) & 0x00ff0000;
    uint32_t patch = array[2] & 0x0000ffff;

    return major | minor | patch;
}
