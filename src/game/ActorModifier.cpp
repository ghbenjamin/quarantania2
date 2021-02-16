#include <game/ActorModifier.h>

ActorModifer::ActorModifer( std::string const &name )
    : m_name(name), m_roundsRemaining(-1), m_hasTimeout(false) {}

ActorModifer::ActorModifer( std::string const &name, int roundsRemaining )
    : m_name(name), m_roundsRemaining(roundsRemaining), m_hasTimeout(true) {}



std::string const &ActorModifer::getName() const
{
    return m_name;
}

bool ActorModifer::hasTimeout() const
{
    return m_hasTimeout;
}

int ActorModifer::getRoundsRemaining() const
{
    return m_roundsRemaining;
}

