#include <game/ActorModifier.h>

ActorModGroup::ActorModGroup( std::string const& id, const std::string &name, int expiryRound)
    : m_id(id), m_name(name), m_expiryRound(expiryRound) {}

std::string const &ActorModGroup::getName() const
{
    return m_name;
}

const int ActorModGroup::getExpiryRound() const
{
    return m_expiryRound;
}

std::vector<ActorMod> const &ActorModGroup::getStatMods() const
{
    return m_statMods;
}

std::string const &ActorModGroup::getId() const
{
    return m_id;
}


ActorMod::ActorMod( ActorModType type, std::string const& id, std::shared_ptr<ActorModImplBase> impl )
 : type(type), id(id), impl(impl) {}
