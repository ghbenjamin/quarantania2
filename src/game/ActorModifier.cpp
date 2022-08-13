#include <game/ActorModifier.h>
#include <game/Actor.h>

// Actor Calc Item
// ---------------------------------------------------------

ActorCalcItem::ActorCalcItem( ActorCalcOperation type, double value )
        : type(type), value(value) {}


// Actor Calc List
// ---------------------------------------------------------


int ActorCalcList::calculate() const
{
    double addBonus = 0;
    double multBonus = 1;

    std::optional<double> limit;
    std::optional<double> floor;

    for (auto const& item : m_modList)
    {
        switch (item.type)
        {
        case ActorCalcOperation::Add:
            addBonus += item.value;
            break;
        case ActorCalcOperation::Multiply:
            addBonus *= item.value;
            break;
        case ActorCalcOperation::Limit:
            if (limit.has_value())
            {
                limit = std::min(*limit, item.value);
            }
            else
            {
                limit = item.value;
            }
            break;
        case ActorCalcOperation::Floor:
            if (floor.has_value())
            {
                floor = std::max(*floor, item.value);
            }
            else
            {
                floor = item.value;
            }
            break;
        }
    }


    double final = 0;
    final += addBonus;
    final *= multBonus;

    if (limit.has_value())
    {
        final = std::min(final, *limit);
    }

    if (floor.has_value())
    {
        final = std::max(final, *floor);
    }

    return (int) final;
}


void ActorCalcList::addItem( ActorCalcOperation type, int value )
{
    addItem(type, (double) value);
}

void ActorCalcList::addItem( ActorCalcOperation type, double value )
{
    m_modList.emplace_back( type, value );
}



// Actor Mod Group
// ---------------------------------------------------------



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

std::string const &ActorModGroup::getId() const
{
    return m_id;
}




// Actor Modifiable Roll Visitor
// ---------------------------------------------------------


ModifiableRollVisitor::ModifiableRollVisitor( Actor const* actor )
        : m_actor(actor) {}

void ModifiableRollVisitor::operator()( ActorCalc::AttackRoll *roll )
{
    m_actor->modifyTypedRoll(ActorCalculationType::AttackRoll, roll );
}

void ModifiableRollVisitor::operator()( ActorCalc::DamageRoll *roll )
{
    m_actor->modifyTypedRoll(ActorCalculationType::DamageRoll, roll );
}

void ModifiableRollVisitor::operator()( ActorCalc::SavingThrowRoll *roll )
{
    m_actor->modifyTypedRoll(ActorCalculationType::SavingThrow, roll );
}

void ModifiableRollVisitor::operator()( ActorCalc::AbilityScoreBonus *roll )
{
    m_actor->modifyTypedRoll(ActorCalculationType::AbilityScore, roll );
}

void ModifiableRollVisitor::operator()( ActorCalc::MovementSpeedData *roll )
{
    m_actor->modifyTypedRoll(ActorCalculationType::MovementSpeed, roll );
}

void ModifiableRollVisitor::operator()(ActorCalc::ArmourClassData *data)
{
    m_actor->modifyTypedRoll(ActorCalculationType::ArmourClass, data );
}

void ModifiableRollVisitor::operator()( ActorCalc::ActionSpeedData *data )
{
    m_actor->modifyTypedRoll(ActorCalculationType::ActionSpeed, data );
}




// Actor Dynamic Mod
// ---------------------------------------------------------


ActorDynamicMod::ActorDynamicMod( ActorCalculationType type, std::string const& id, std::shared_ptr<ActorStatDynamicImplBase> impl )
 : type(type), id(id), impl(impl) {}

ActorActionMod::ActorActionMod( std::string const &id, GameAction const &action )
 : id(id), action(action) {}

