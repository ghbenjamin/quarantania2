#include <utils/Random.h>

#include <sstream>

#include <utils/Assert.h>
#include <game/Defines.h>


RandomState::RandomState( std::string const &seed, std::string const& prngState )
    : m_seed(seed)
{
    std::seed_seq seedSeq(m_seed.begin(), m_seed.end());
    m_mt = std::mt19937{ seedSeq };
    
    if ( !prngState.empty() )
    {
        std::stringstream ss;
        ss.str(prngState);
        ss >> m_mt;
    }
}

RandomState::RandomState()
: RandomState(  RandomState::generateSeed(), "" )
{}

bool RandomState::weightedFlip( int odds)
{
    std::uniform_int_distribution<> dis(1, odds);
    return dis(m_mt) == 1;
}

bool RandomState::coinflip()
{
    return std::uniform_int_distribution<>(1, 2)(m_mt) == 1;
}

int RandomState::diceRoll( int dcount, int dsize)
{
    std::uniform_int_distribution dist(1, dsize);
    int out = 0;

    for ( int i = 0; i < dcount; i++ )
    {
        out += dist(m_mt);
    }

    return out;
}

int RandomState::diceRoll( const DiceRoll &roll)
{
    return diceRoll(roll.diceCount, roll.diceSize) + roll.modifier;
}

int RandomState::diceRoll( int size)
{
    return diceRoll(1, size);
}

int RandomState::randomInt( int lower, int upper)
{
    std::uniform_int_distribution<> dist( lower, upper );
    return dist( m_mt );
}

const std::string RandomState::SeedChars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";


std::string RandomState::generateSeed()
{
    std::string seed;
    std::random_device rd;
    std::mt19937 twister( rd() );
    
    seed.resize(SeedSize, ' ');
    
    std::uniform_int_distribution<> seedGen( 0, SeedChars.size() );
    
    for ( int i = 0; i < SeedSize; i++ )
    {
        auto idx = seedGen( twister );
        seed[i] = SeedChars[idx];
    }
    
    return seed;
}

std::string const &RandomState::seed() const
{
    return m_seed;
}

std::string RandomState::prngState() const
{
    std::stringstream ss;
    ss << m_mt;
    
    return ss.str();
}
