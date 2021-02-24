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

std::vector<ActorStatMod> const &ActorModGroup::getStatMods() const
{
    return m_statMods;
}

std::vector<ActorActionMod> const& ActorModGroup::getActionMods() const
{
    return m_actionMods;
}

std::string const &ActorModGroup::getId() const
{
    return m_id;
}

ActorStatMod::ActorStatMod(ActorStatModType type, std::string const& id, std::shared_ptr<ActorStatModImplBase> impl )
 : type(type), id(id), impl(impl) {}

ActorActionMod::ActorActionMod( std::string const &id, GameAction const &action )
 : id(id), action(action) {}
