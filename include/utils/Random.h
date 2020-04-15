#pragma once

#include <random>

using RandomGenerator = std::mt19937;

template <typename It>
It randomElement(It begin, It end, RandomGenerator& rd)
{
    std::uniform_int_distribution<> distribution(0, std::distance(begin, end) - 1);
    std::advance(begin, distribution(rd));
    return begin;
}

inline bool weightedFlip( int odds, RandomGenerator& rd )
{
    std::uniform_int_distribution<> dis(1, odds);
    return dis(rd) == 1;
}

inline bool coinflip( RandomGenerator& rg )
{
    return std::uniform_int_distribution<>(1, 2)(rg) == 1;
}

inline int diceroll( int dcount, int dsize, RandomGenerator& rd )
{
    std::uniform_int_distribution dist(1, dsize);
    int out = 0;

    for ( int i = 0; i < dcount; i++ )
    {
        out += dist(rd);
    }

    return out;
}