#include "StringHelper.h"

std::string StringHelper::ReplaceAll(const std::string& input, const std::string& replace, const std::string& value)
{
    std::string output = input;
    while (true)
    {
        auto index = output.find(replace);
        if (index == std::string::npos) break;

        output.replace(index, replace.length(), value);
    }
    return output;
}
