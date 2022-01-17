#include "helper.h"

#include <sstream>

std::vector<std::string> Helper::split(const std::string& in, const char delimiter)
{
    std::vector<std::string> output;
    std::istringstream stream(in);
    std::string str;

    while (std::getline(stream, str, delimiter))
        output.push_back(str);

    return output;
}

uint32_t Helper::toInt(const std::string& version)
{
    std::vector<std::string> list = split(version, '.');

    uint32_t major = (std::stoi(list[0]) << 24) & 0xff000000;
    uint32_t minor = (std::stoi(list[1]) << 16) & 0x00ff0000;
    uint32_t patch = (std::stoi(list[2]) << 0) & 0x0000ffff;

    return major | minor | patch;
}
