#include <game/ActorModifier.h>
#include <game/Actor.h>


double ActorModifiableData::calculate() const
{
    double addBonus = 0;
    double multBonus = 1;
    
    std::optional<double> limit;
    std::optional<double> floor;
    
    for ( auto const& item : m_modComponents )
    {
        switch ( item.type )
        {
            case ModComponentType::Add:
                addBonus += item.value;
                break;
            case ModComponentType::Multiply:
                addBonus *= item.value;
                break;
            case ModComponentType::Limit:
                if ( limit.has_value() )
                {
                    limit = std::min( *limit, item.value );
                }
                else
                {
                    limit = item.value;
                }
                break;
            case ModComponentType::Floor:
                if ( floor.has_value() )
                {
                    floor = std::max( *floor, item.value );
                }
                else
                {
                    floor = item.value;
                }
                break;
        }
    }
    
    
    double final = m_baseValue;
    final += addBonus;
    final *= multBonus;
    
    if ( limit.has_value() )
    {
        final = std::min( final, *limit );
    }
    
    if ( floor.has_value() )
    {
        final = std::max( final, *floor );
    }
    
    return final;
}

void ActorModifiableData::addModComponent( ModComponentType type, int value )
{
    addModComponent( type, (double) value );
}

void ActorModifiableData::addModComponent( ModComponentType type, double value )
{
    m_modComponents.push_back({ type, value });
}

ActorModifiableData::ActorModifiableData( Actor *target, int baseValue, EntityRef source )
    : m_actor(target), m_baseValue(baseValue), m_target(target->getRef()), m_other(source) {}

ActorModifiableData::ActorModifiableData( Actor *target, int baseValue )
 : ActorModifiableData( target, baseValue, EntityNull ) {}

int ActorModifiableData::baseValue() const
{
    return m_baseValue;
}

EntityRef ActorModifiableData::target() const
{
    return m_target;
}

EntityRef ActorModifiableData::source() const
{
    return m_other;
}

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
