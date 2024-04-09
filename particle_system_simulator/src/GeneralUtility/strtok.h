#pragma once

#include <string_view>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(const std::string& str, char delimiter)
{
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;
    while(std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> tokenize(std::string_view str, char delimiter)
{
    std::stringstream ss(str.data());
    std::string token;
    std::vector<std::string> tokens;
    while(std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}