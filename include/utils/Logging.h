#pragma once

#include <iostream>


#define LOG_VAR(v) std::cout << #v << " " << v << std::endl


class Logging
{
public:

    template <typename T>
    static void log(T val)
    {
        std::cout << std::to_string(val) << std::endl;
    }
};