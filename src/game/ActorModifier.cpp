#include <game/ActorModifier.h>

MeleeAttackCountItem::MeleeAttackCountItem( Weapon const *weapon, int naturalAttackMod )
        : weapon(weapon), naturalAttackMod(naturalAttackMod) {}

ActorModGroup::ActorModGroup( std::string const& id, const std::string &name, int expiryRound)
    : m_id(id), m_name(name), m_expiryRound(expiryRound) {}

std::string const &ActorModGroup::getName() const
{
    return m_name;
}

int ActorModGroup::getExpiryRound() const
{
    return m_expiryRound;
}

std::vector<ActorDynamicMod> const &ActorModGroup::getDynamicMods() const
{
    return m_dynamicMods;
}

std::vector<ActorActionMod> const& ActorModGroup::getActionMods() const
{
    return m_actionMods;
}

std::vector<ActorStaticMod> const &ActorModGroup::getStaticMods() const
{
    return m_staticMods;
}

std::string const &ActorModGroup::getId() const
{
    return m_id;
}

ActorDynamicMod::ActorDynamicMod( ActorDynamicModType type, std::string const& id, std::shared_ptr<ActorStatDynamicImplBase> impl )
 : type(type), id(id), impl(impl) {}

ActorActionMod::ActorActionMod( std::string const &id, GameAction const &action )
 : id(id), action(action) {}

ActorStaticMod::ActorStaticMod( std::string const &id, ActorStaticModType type, int value)
: type(type), value(value), id(id) {}
