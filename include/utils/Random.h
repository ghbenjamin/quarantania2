#pragma once

#include <random>

template <typename It>
It randomElement(It begin, It end, std::mt19937 &rd)
{
    std::uniform_int_distribution<> distribution(0, std::distance(begin, end) - 1);
    std::advance(begin, distribution(rd));
    return begin;
}

inline bool weightedFlip( int odds, std::mt19937& rd )
{
    std::uniform_int_distribution<> dis(1, odds);
    return dis(rd) == 1;
}