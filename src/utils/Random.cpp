#include <utils/Random.h>

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

int RandomInterface::diceroll(int dcount, int dsize)
{
    std::uniform_int_distribution dist(1, dsize);
    int out = 0;

    for ( int i = 0; i < dcount; i++ )
    {
        out += dist(m_mt);
    }

    return out;
}

RandomGenerator &RandomInterface::generator()
{
    return m_mt;
}

int RandomInterface::randomInt(int lower, int upper)
{
    std::uniform_int_distribution<> dist( lower, upper );
    return dist( m_mt );
}
