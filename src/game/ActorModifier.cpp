#include <game/ActorModifier.h>

ActorModGroup::ActorModGroup(const std::string &name, int expiryRound)
    : m_name(name), m_expiryRound(expiryRound) {}

std::string const &ActorModGroup::getName() const
{
    return m_name;
}

const int ActorModGroup::getExpiryRound() const
{
    return m_expiryRound;
}


bool ActorModCompare::operator()(const ActorModGroup &lhs, const ActorModGroup &rhs) const
{
    return lhs.getExpiryRound() > rhs.getExpiryRound();
}
