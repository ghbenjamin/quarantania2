#include <game/ActorModifier.h>

ActorMod::ActorMod(const std::string &name, int expiryRound, std::unique_ptr<ActorModImpl> impl)
    : m_name(name), m_expiryRound(expiryRound)//, m_impl( std::move(impl) )
{
}

std::string const &ActorMod::getName() const
{
    return m_name;
}

const int ActorMod::getExpiryRound() const
{
    return m_expiryRound;
}

bool ActorModCompare::operator()(const ActorMod &lhs, const ActorMod &rhs) const
{
    return lhs.getExpiryRound() > rhs.getExpiryRound();
}
