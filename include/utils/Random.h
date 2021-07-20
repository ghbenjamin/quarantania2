#pragma once

#include <random>
#include <algorithm>

struct DiceRoll;


class RandomState
{
public:

    RandomState();
    RandomState( std::string const& seed, std::string const& prngState );

    ~RandomState() = default;
    
    std::string const& seed() const;
    std::string prngState() const;
    
    
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
    
    
private:

    static std::string generateSeed();
    static const std::string SeedChars;
    static constexpr int SeedSize = 24;

    std::mt19937 m_mt;
    std::string m_seed;

};