#include "StringHelper.h"

std::string StringHelper::ReplaceAll(std::string input, std::string replace, std::string value)
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
