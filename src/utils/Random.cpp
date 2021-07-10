#include <utils/Random.h>
#include <utils/Assert.h>

RandomInterface::RandomInterface(RandomGenerator rg)
        : m_mt(rg) { }

bool RandomInterface::weightedFlip(int odds)
{
    std::uniform_int_distribution<> dis(1, odds);
    return dis(m_mt) == 1;
}

bool RandomInterface::coinflip()
{
    return std::uniform_int_distribution<>(1, 2)(m_mt) == 1;
}

int RandomInterface::diceRoll(int dcount, int dsize)
{
    std::uniform_int_distribution dist(1, dsize);
    int out = 0;

    for ( int i = 0; i < dcount; i++ )
    {
        out += dist(m_mt);
    }

    return out;
}

int RandomInterface::diceRoll(const DiceRoll &roll)
{
    return diceRoll(roll.diceCount, roll.diceSize) + roll.modifier;
}

int RandomInterface::diceRoll(int size)
{
    return diceRoll(1, size);
}

int RandomInterface::randomInt(int lower, int upper)
{
    std::uniform_int_distribution<> dist( lower, upper );
    return dist( m_mt );
}

const std::string RandomSeed::SeedChars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";

RandomSeed::RandomSeed()
{


    std::random_device rd;
    std::mt19937 twister( rd() );

    m_seed.resize(SeedSize, ' ');

    std::uniform_int_distribution<> seedGen( 0, SeedChars.size() );

    for ( int i = 0; i < SeedSize; i++ )
    {
        auto idx = seedGen( twister );
        m_seed[i] = SeedChars[idx];
    }
}

std::string const &RandomSeed::seed() const
{
    return m_seed;
}

RandomSeed::RandomSeed(const std::string &seed )
    : m_seed(seed)
{
    Assert( seed.size() == SeedSize );
}
