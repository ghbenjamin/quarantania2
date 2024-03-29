#include <game/Actor.h>
#include <game/Level.h>
#include <game/Attack.h>
#include <game/RunState.h>
#include <game/ActionDefs.h>


Actor::Actor( Level *level, EntityRef ref, ActorData const &pdata )
        : m_level(level),
          m_entity(ref),
          m_name(pdata.name),
          m_baseAbilityScoreStr(pdata.attrStr),
          m_baseAbilityScoreDex(pdata.attrDex),
          m_baseAbilityScoreCon(pdata.attrCon),
          m_baseAbilityScoreInt(pdata.attrInt),
          m_baseAbilityScoreWis(pdata.attrWis),
          m_baseAbilityScoreCha(pdata.attrCha),
          m_baseFortitude(pdata.saveFort),
          m_baseReflex(pdata.saveRef),
          m_baseWill(pdata.saveWill),
          m_HpMax(pdata.maxHP),
          m_HpCurrent(pdata.maxHP),
          m_baseSpeed(pdata.baseSpeed),
          m_size(pdata.size),
          m_creatureType(pdata.creatureType),
          m_usedActions(0),
          m_maxActions(3)
{
}

std::string const& Actor::getName() const
{
    return m_name;
}

std::string const &Actor::getCreatureType() const
{
    return m_creatureType;
}

bool Actor::hasEquipped(CreatureEquipSlot slot) const
{
    return m_equippedItems.find(slot) != m_equippedItems.end();
}

ItemPtr Actor::getEquipped(CreatureEquipSlot slot) const
{
    auto it = m_equippedItems.find(slot);
    if (it != m_equippedItems.end() )
    {
        return it->second;
    }
    else
    {
        return ItemPtr();
    }
}

ItemPtr Actor::unequipItem(CreatureEquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

ItemPtr Actor::equipItem(CreatureEquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    m_equippedItems.emplace(slot, item);

    return lastEquipped;
}


std::pair<Weapon const*, Weapon const*> Actor::getEquippedWeapons() const
{
    Weapon const* mainHand = nullptr;
    Weapon const* offHand = nullptr;
    
    // If we have a weapon in the right hand, assume it's the main weapon
    if ( hasEquipped( CreatureEquipSlot::RightHand ) )
    {
        auto right = getEquipped( CreatureEquipSlot::RightHand );
        if ( right->getEquipSlot() == ItemEquipSlot::Weapon )
        {
            mainHand =  &right->getWeapon();
        }
    }
    
    // If there's a weapon in the left hand, assume it's the off weapon if there's a right hand weapon
    // Otherwise, assume it's the main weapon
    if ( hasEquipped( CreatureEquipSlot::LeftHand ) )
    {
        auto left = getEquipped( CreatureEquipSlot::LeftHand );
        if ( left->getEquipSlot() == ItemEquipSlot::Weapon )
        {
            if (mainHand != nullptr)
            {
                offHand = &left->getWeapon();
            }
            else
            {
                mainHand = &left->getWeapon();
            }
        }
    }
    
    if (mainHand == nullptr)
    {
        mainHand = &getNaturalWeapon();
    }
    
    return std::make_pair( mainHand, offHand );
}


Armour const* Actor::tryGetActiveShield() const
{
    if ( hasEquipped( CreatureEquipSlot::RightHand ) )
    {
        auto right = getEquipped( CreatureEquipSlot::RightHand );
        if ( right->getEquipSlot() == ItemEquipSlot::Shield )
        {
            return &right->getArmour();
        }
    }

    else if ( hasEquipped( CreatureEquipSlot::LeftHand ) )
    {
        auto left = getEquipped( CreatureEquipSlot::LeftHand );
        if ( left->getEquipSlot() == ItemEquipSlot::Shield )
        {
            return &left->getArmour();
        }
    }

    return nullptr;
}


Weapon const& Actor::getNaturalWeapon() const
{
    if ( m_naturalWeapons.empty() )
    {
        return WeaponUnarmedAttack;
    }

    // TODO Don't just return the front weapon
    return m_naturalWeapons.front();
}

int Actor::getCurrentHp() const
{
    return m_HpCurrent;
}

int Actor::getMaxHp() const
{
    return m_HpMax;
}

HealthLevel Actor::getHealthLevel() const
{
    float hpPercentage = (100.f * m_HpCurrent) / m_HpMax;
    
    if ( hpPercentage < 10 )
    {
        return HealthLevel::Near_Death;
    }
    else if ( hpPercentage < 30 )
    {
        return HealthLevel::Badly_Injured;
    }
    else if ( hpPercentage < 70 )
    {
        return HealthLevel::Injured;
    }
    else if ( hpPercentage < 95 )
    {
        return HealthLevel::Barely_Injured;
    }
    else
    {
        return HealthLevel::Uninjured;
    }
}

void Actor::setCurrentHp(int value)
{
    m_HpCurrent = value;
}

int Actor::getSpeed()
{
    return getModifiersMovementSpeed().mods.calculate( m_baseSpeed );
}

CreatureSize Actor::getSize()
{
    return m_size;
}

float Actor::getReach() const
{
    // TODO Modifiers
    return 1.5f;
}

std::optional<CreatureEquipSlot> Actor::defaultSlotForItemSlot(ItemEquipSlot slot) const
{
    switch( slot )
    {
        case ItemEquipSlot::Armor:
            return CreatureEquipSlot::Armour;
        case ItemEquipSlot::Arms:
            return CreatureEquipSlot::Arms;
        case ItemEquipSlot::Belt:
            return CreatureEquipSlot::Belt;
        case ItemEquipSlot::Body:
            return CreatureEquipSlot::Body;
        case ItemEquipSlot::Feet:
            return CreatureEquipSlot::Feet;
        case ItemEquipSlot::Hands:
            return CreatureEquipSlot::RightHand;
        case ItemEquipSlot::Head:
            return CreatureEquipSlot::Head;
        case ItemEquipSlot::Neck:
            return CreatureEquipSlot::Neck;
        case ItemEquipSlot::Ring:
            return CreatureEquipSlot::Ring1;
        case ItemEquipSlot::Shield:
            return CreatureEquipSlot::LeftHand;
        case ItemEquipSlot::Shoulders:
            return CreatureEquipSlot::Shoulders;
        case ItemEquipSlot::Weapon:
            return CreatureEquipSlot::RightHand;
        default:
            return {};
    }
}

std::unordered_map<CreatureEquipSlot, ItemPtr> const &Actor::getAllEquippedItems() const
{
    return m_equippedItems;
}

ActorCalcData Actor::getModifiersCritRange(  Actor* defender, const Weapon* weapon  ) const
{
    ActorCalc::CritRange critRangeCtx { defender, weapon };
    
    ActorCalcData critData;
    critData.actor = this;
    critData.type = ActorCalculationType::CritRange;
    critData.data = critRangeCtx;
    
    applyAllModifiers( critData );
    
    return critData;
}

ActorCalcData Actor::getModifiersMeleeDamage( ActorCalc::DamageRoll const& ctx, std::shared_ptr<MeleeAttack> attackImpl ) const
{
    ActorCalcData data;
    data.actor = this;
    data.type = ActorCalculationType::DamageRoll;
    data.data = ctx;
    
    data.mods.addItem( ActorCalcOperation::Add, "Strength Bonus", getAbilityScoreMod(AbilityScoreType::STR) );
    
    // Apply modifiers from the attack type if any (e.g. +dmg from a power attack)
    attackImpl->modifyDamageRoll( data );
    
    // Apply modifiers from the actor
    applyAllModifiers( data );
    
    return data;
}


ActorCalcData Actor::getModifiersMovementSpeed() const
{
    ActorCalcData data;
    data.actor = this;
    data.type = ActorCalculationType::MovementSpeed;
    data.data = ActorCalc::MovementSpeed();
    
    applyAllModifiers(data);
    
    return data;
}

ActorCalcData Actor::getModifiersActionSpeed( const GameAction* action ) const
{
    ActorCalcData data;
    data.actor = this;
    data.data = ActorCalc::ActionSpeed { action };
    
    applyAllModifiers( data );
    
    return data;
}



void Actor::acceptDamage( Damage const &dmg )
{
    // Decrease our HP for each instance of damage supplied
    // TODO: Modifiers
    
    int oldCurrHp = getCurrentHp();
    int totalDamage = 0;
    
    for (auto const& instance : dmg.instances )
    {
        totalDamage += instance.total;
    }
    
    int newCurrHp = oldCurrHp - totalDamage;
    setCurrentHp(newCurrHp);
    
    Logger::get().info( "Damage dealt: damage={}, old hp={}, new hp={}", totalDamage, oldCurrHp, newCurrHp );
    
    // Handle falling unconsious and death
    
    if ( getCurrentHp() <= 0 )
    {
        m_level->events().broadcast<GameEvents::EntityDeath>( m_entity );
    }
}

void Actor::addModifierGroup( ActorModGroup const &modGroup )
{
    m_modifierGroups.push_back(modGroup);
    
    for ( auto const& mod : modGroup.getDynamicMods() )
    {
        m_dynamicModifiers.emplace(mod.type, mod );
    }
    
    for (auto const& mod : modGroup.getActionMods())
    {
        m_actionMods.push_back( mod );
    }
}

void Actor::removeActorModGroup( std::string const& id )
{
    // Remove the dynamic and static modifiers
    
    auto dynIt = m_dynamicModifiers.begin();
    if ( ; dynIt != m_dynamicModifiers.end() )
    {
        if ( dynIt->second.id == id )
        {
            dynIt = m_dynamicModifiers.erase(dynIt);
        }
        else
        {
            dynIt++;
        }
    }

    // Remove the modifier group
    m_modifierGroups.erase( std::remove_if( m_modifierGroups.begin(), m_modifierGroups.end(), [&id](auto const& item) {
        return item.getId() == id;
    }), m_modifierGroups.end() );
}


void Actor::applyAllModifiers( ActorCalcData& data ) const
{
    auto range = m_dynamicModifiers.equal_range( data.type );
    for ( auto it = range.first; it != range.second; it++ )
    {
        it->second.impl->modify( data );
    }}


ActorCalcData Actor::getModifiersSavingThrow( SavingThrowType type, EntityRef source ) const
{
    AbilityScoreType abilityScore;

    switch (type)
    {
        case SavingThrowType::Reflex:
            abilityScore = AbilityScoreType::DEX;
            break;
        case SavingThrowType::Fortitude:
            abilityScore = AbilityScoreType::CON;
            break;
        case SavingThrowType::Will:
            abilityScore = AbilityScoreType::WIS;
            break;
    }
    
    int abilityScoreMod = getAbilityScoreMod( abilityScore );
    
    
    ActorCalcData data;
    data.actor = this;
    data.type = ActorCalculationType::SavingThrow;
    data.data = ActorCalc::SavingThrowRoll{ type, source };
    
    data.mods.addItem(ActorCalcOperation::Add, "Ability Score", abilityScoreMod);

    applyAllModifiers( data );

    return data;
}


ActorCalcData Actor::getModifiersAttackRoll( Actor *defender, const Weapon *weapon, std::shared_ptr<MeleeAttack> attackImpl ) const
{
    ActorCalc::AttackRoll attackRollCtx { defender, weapon };
    
    ActorCalcData attackData;
    attackData.actor = this;
    attackData.type = ActorCalculationType::AttackRoll;
    attackData.data = attackRollCtx;
    
    // Add STR mod to the attack roll (TODO: This should be modifible, e.g. Weapon Finesse)
    attackData.mods.addItem(ActorCalcOperation::Add, "Strength Bonus", getAbilityScoreValue(AbilityScoreType::STR));
    
    // Apply any modifiers from the type of attack, e.g. reduce to hit from a Power Attack
    attackImpl->modifyAttackRoll( attackData );
    
    // Apply any modifiers from the actors, e.g. Weapon Focus feats or status affects
    applyAllModifiers( attackData );
    
    return attackData;
}


int Actor::getAbilityScoreMod( AbilityScoreType type ) const
{
    return std::floor( ( getAbilityScoreValue( type ) - 10 ) / 2 );
}

int Actor::getAbilityScoreValue( AbilityScoreType type ) const
{
    int base;
    
    switch(type)
    {
        case AbilityScoreType::STR:
            base = m_baseAbilityScoreStr;
            break;
        case AbilityScoreType::DEX:
            base = m_baseAbilityScoreDex;
            break;
        case AbilityScoreType::CON:
            base = m_baseAbilityScoreCon;
            break;
        case AbilityScoreType::INT:
            base = m_baseAbilityScoreInt;
            break;
        case AbilityScoreType::WIS:
            base = m_baseAbilityScoreWis;
            break;
        default:
            base = m_baseAbilityScoreCha;
            break;
    }
    
    ActorCalcData data;
    data.actor = this;
    data.type = ActorCalculationType::AbilityScore;
    data.data = ActorCalc::AbilityScoreBonus { type };
    
    applyAllModifiers( data );

    return data.mods.calculate( base );
}

std::vector<GameAction> Actor::getAllGameActions() const
{
    std::vector<GameAction> out;

    // Add the actions which all actors have access to by default

    out.emplace_back(
            "move", TargetingType::SingleTile,
            std::make_shared<ActionMoveStride>()
    );

    out.emplace_back(
            "short-step", TargetingType::SingleTile,
            std::make_shared<ActionMoveStep>()
    );

    out.emplace_back(
            "strike", TargetingType::SingleEntity,
            std::make_shared<ActionMeleeAttack>()
    );

    for (auto const& mod : m_actionMods)
    {
        out.push_back( mod.action );
    }

    for ( auto& act : out )
    {
        act.enabled = canPerformAction( act );
    }

    return out;
}

bool Actor::canPerformAction( GameAction const& action ) const
{

    // TODO REFACTOR
//    ActorCalc::ActionSpeedData speedData { this };
//
//    speedData.action = &action;
//
//
//    applyAllModifiers( &speedData );
    
//    return (m_maxActions - m_usedActions) >= speedData.modified;
    
    return (m_maxActions - m_usedActions) > 1;
}

std::optional<CreatureEquipSlot> Actor::canEquipItem( ItemPtr item )
{
    // For now, always return true.
    return defaultSlotForItemSlot( item->getEquipSlot() );
}

std::vector<ActorModGroup> const &Actor::getAllModifierGroups() const
{
    return m_modifierGroups;
}

int Actor::getMaxActions() const
{
    return m_maxActions;
}

void Actor::resetActions()
{
    m_usedActions = 0;
}

int Actor::getActionsRemaining() const
{
    return m_maxActions - m_usedActions;
}

void Actor::useActions( int count )
{
    m_usedActions += count;
    AssertMsg( m_usedActions <= m_maxActions, "More actions used than possible" );
}

EntityRef Actor::getRef() const
{
    return m_entity;
}







