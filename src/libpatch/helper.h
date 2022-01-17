#ifndef HELPER_H
#define HELPER_H

#include <cstdint>
#include <string>
#include <vector>

class Helper
{
public:
    static std::vector<std::string> split(const std::string& in, const char delimiter);
    static uint32_t toInt(const std::string& version);
};

#endif // HELPER_H
