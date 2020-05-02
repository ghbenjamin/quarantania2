#pragma once

#include <random>

using RandomGenerator = std::mt19937;

class RandomInterface
{
public:
    RandomInterface(RandomGenerator rg);
    ~RandomInterface() = default;

    template <typename It>
    It randomElement(It begin, It end)
    {
        std::uniform_int_distribution<> distribution(0, std::distance(begin, end) - 1);
        std::advance(begin, distribution(m_mt));
        return begin;
    }

    bool weightedFlip(int odds);
    bool coinflip();
    int diceroll(int dcount, int dsize);

    RandomGenerator& generator();

private:
    RandomGenerator m_mt;

};