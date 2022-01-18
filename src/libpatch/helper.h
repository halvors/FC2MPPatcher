#ifndef HELPER_H
#define HELPER_H

#include <cstdint>
#include <string>

class Helper
{
public:
    static uint32_t toInt(const std::string& version);
};

#endif // HELPER_H
