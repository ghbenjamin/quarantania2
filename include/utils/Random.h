#pragma once

#include <random>

template <typename It>
It random_element(It begin, It end, std::mt19937& rd)
{
    std::uniform_int_distribution<> distribution(0, std::distance(begin, end) - 1);
    std::advance(begin, distribution(rd));
    return begin;
}