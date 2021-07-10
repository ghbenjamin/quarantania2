#pragma once

#include <random>
#include <algorithm>

#include <game/Defines.h>

using RandomGenerator = std::mt19937;

class RandomInterface
{
public:
    RandomInterface(RandomGenerator rg);
    ~RandomInterface() = default;

    // Return a random iterator between the given ranges (inclusive)
    template <typename It>
    It randomElement(It begin, It end)
    {
        std::uniform_int_distribution<> distribution(0, std::distance(begin, end) - 1);
        std::advance(begin, distribution(m_mt));
        return begin;
    }

    // Return a random iterator within the supplied container.
    template <typename T>
    auto randomElement( T const& t )
    {
        return randomElement( t.begin(), t.end() );
    }

    // Shuffle the given STL container
    template <typename T>
    void shuffle( T& t )
    {
        std::shuffle( t.begin(), t.end(), m_mt );
    }

    // Flip a weighted coin which returns true 1 in [odds] times.
    bool weightedFlip(int odds);

    // Return true 50% of the time.
    bool coinflip();

    // Return the value of rolling [dcount] 1d[dsize] dice.
    int diceRoll(int dcount, int dsize);
    int diceRoll(int size);
    int diceRoll(DiceRoll const& roll);

    // Return a random int between the given ranges (inclusive).
    int randomInt( int lower, int upper );


private:

    // Cache our supplied random engine to make seeding easier
    RandomGenerator m_mt;

};

class RandomSeed
{
public:
    RandomSeed( std::string const& seed );
    RandomSeed();
    ~RandomSeed() = default;

    std::string const& seed() const;

private:
    static const std::string SeedChars;
    static constexpr int SeedSize = 24;

    std::string m_seed;
};