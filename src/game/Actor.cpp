#include <game/Actor.h>
#include <game/Level.h>
#include <game/Attack.h>
#include <game/RunState.h>

Actor::Actor(Level* level, EntityRef ref, CreatureData const& rcd)
    : m_level(level),
      m_entity(ref),
      m_name(rcd.name),
      m_baseAbilityScoreStr(rcd.attrStr),
      m_baseAbilityScoreDex(rcd.attrDex),
      m_baseAbilityScoreCon(rcd.attrCon),
      m_baseAbilityScoreInt(rcd.attrInt),
      m_baseAbilityScoreWis(rcd.attrWis),
      m_baseAbilityScoreCha(rcd.attrCha),
      m_baseFortitude(rcd.saveFort),
      m_baseReflex(rcd.saveRef),
      m_baseWill(rcd.saveWill),
      m_HpMax(rcd.maxHP),
      m_HpCurrent(rcd.maxHP),
      m_baseSpeed(rcd.speed),
      m_size(rcd.size),
      m_creatureType(rcd.creatureType)
{
}

Actor::Actor(Level* level, EntityRef ref, PlayerData const &pdata)
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
      m_size(CreatureSize::Medium),
      m_creatureType("Humanoid")
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

const ItemPtr Actor::getEquipped(CreatureEquipSlot slot) const
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

int Actor::getAC() const
{
    ArmourClassData data;
    data.dexBonus = getModDex();

    if ( hasEquipped(CreatureEquipSlot::Armour) )
    {
        auto const& armour = getEquipped( CreatureEquipSlot::Armour )->getArmour();

        data.maxDexToAc = armour.maxDex();
        data.armourBonus = armour.armourBonus();
    }

    auto shield = tryGetActiveShield();

    if ( shield != nullptr )
    {
        if (data.maxDexToAc.has_value())
        {
            data.maxDexToAc = std::min( *data.maxDexToAc, shield->maxDex() );
        }
        else
        {
            data.maxDexToAc = shield->maxDex();
        }

        data.shieldBonus = shield->armourBonus();
    }

    applyAllModifiers( &data );

    return data.armourBonus + data.shieldBonus + data.staticBonus + data.dexBonus + data.dodgeBonus;
}


int Actor::getSpeed() const
{
    MovementSpeedData data;
    data.natural = m_baseSpeed;
    data.modified = m_baseSpeed;
    
    applyAllModifiers(&data);
    
    int speed = data.modified;
    
    if ( data.moveSpeedLimit >= 0 )
    {
        speed = std::min( speed, data.moveSpeedLimit );
    }
    
    speed = (int)(speed * data.multiplier);
    
    return speed;
}

CreatureSize Actor::getSize()
{
    // TODO Modifiers
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

int Actor::getCritRangeForAttack( SingleMeleeAttackInstance &attack ) const
{
    // TODO: Modifiers.
    return attack.weapon->critRange();
}

DamageRoll Actor::makeMeleeDamageRoll( SingleMeleeAttackInstance &attack, std::shared_ptr<MeleeAttack> attackImpl, AttackRoll const &roll ) const
{
    DamageRoll damageRoll;
    
    // Roll the natural roll
    damageRoll.naturalRoll =  m_level->random()->diceRoll( attack.weapon->damage() );
    damageRoll.modifiedRoll = damageRoll.naturalRoll;
    
    // Add STR mod to the damage roll (TODO: This should be modifible)
    damageRoll.modifiedRoll += getModStr();
    
    
    // Apply modifiers from the attack type if any (e.g. +dmg from a power attack)
    attackImpl->modifyDamageRoll(damageRoll);
    
    // Apply modifiers from the actors
    applyAllModifiers( &damageRoll );
    
    // If this is a critical hit, modify the damage
    if ( roll.isCrit )
    {
        damageRoll.modifiedRoll *= attack.weapon->critMultiplier();
    }
    
    return damageRoll;
}

int Actor::getAcForMeleeAttack( SingleMeleeAttackInstance &attack, std::shared_ptr<MeleeAttack> attackImpl) const
{
    // TODO: All the modifiers
    return getAC();
}

AttackRoll Actor::makeMeleeAttackRoll( SingleMeleeAttackInstance &attack, std::shared_ptr<MeleeAttack> attackImpl, bool isCritConfirm ) const
{
    AttackRoll result;
    result.ctx = &attack;
    result.naturalRoll = m_level->random()->diceRoll(20);
    result.targetValue = attack.defender->getAcForMeleeAttack(attack, attackImpl);
    int critRange = getCritRangeForAttack( attack );

    // Start with the natural roll
    result.modifiedRoll = result.naturalRoll;
    
    // Add STR mod to the attack roll (TODO: This should be modifible, e.g. Weapon Finesse)
    result.modifiedRoll += getModStr();
    
    // Apply any modifiers from the type of attack, e.g. reduce to hit from a Power Attack
    attackImpl->modifyAttackRoll( result );

    // Apply any modifiers from the actors, e.g. Weapon Focus feats or status affects
    applyAllModifiers( &result );

    if ( result.naturalRoll >= critRange )
    {
        // Potential crit
        if ( !isCritConfirm )
        {
            // TODO: look up whether crit confirms are made with the same attack feat as the original attack
            auto confirmResult = makeMeleeAttackRoll(attack, attackImpl, true);
            if ( confirmResult.isHit )
            {
                // Confirmed crit - a hit and crit
                // Trigger: crit confirmed
                result.isCrit = true;
                result.isHit = true;
            }
        }
        else
        {
            // Crit confirmation failed, but still a guaranteed hit
            // Trigger: crit confirmation failed
            result.isHit = true;
            result.isCrit = false;
        }
    }
    else
    {
        // Not a crit - compare the roll against the target value
        if ( result.modifiedRoll >= result.targetValue )
        {
            // A hit!
            // Trigger: successful attack roll
            result.isHit = true;
        }
        else
        {
            // A miss
            // Trigger: unsuccessful attack roll
            result.isHit = false;
        }
    }
    
    return result;
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
        if ( /*getCurrentHp() > -getAbilityScoreValue(AbilityScoreType::CON)*/ false )
        {
            // Unconscious! TODO
        }
        else
        {
            // The entity died
            m_level->events().broadcast<GameEvents::EntityDeath>( m_entity );
        }
    }
}

void Actor::addModifierGroup( ActorModGroup const &modGroup )
{
    m_modifierGroups.push_back(modGroup);
    
    for ( auto const& mod : modGroup.getDynamicMods() )
    {
        m_dynamicModifiers.emplace(mod.type, mod );
    }
    
    for ( auto const& mod : modGroup.getStaticMods() )
    {
        m_staticModifiers.emplace(mod.type, mod );
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
    
    auto statIt = m_staticModifiers.begin();
    if ( ; statIt != m_staticModifiers.end() )
    {
        if ( statIt->second.id == id )
        {
            statIt = m_staticModifiers.erase(statIt);
        }
        else
        {
            statIt++;
        }
    }

    // Remove the modifier group
    m_modifierGroups.erase( std::remove_if( m_modifierGroups.begin(), m_modifierGroups.end(), [&id](auto const& item) {
        return item.getId() == id;
    }), m_modifierGroups.end() );
}


void Actor::applyAllModifiers(ModifiableStatObject roll ) const
{
    std::visit( ModifiableRollVisitor{this}, roll );
}

SavingThrowRoll Actor::makeSavingThrow(EntityRef source, SavingThrowType type) const
{
    int abilityScoreMod;

    switch (type)
    {
        case SavingThrowType::Reflex:
            abilityScoreMod = getModDex();
            break;
        case SavingThrowType::Fortitude:
            abilityScoreMod = getModCon();
            break;
        case SavingThrowType::Will:
            abilityScoreMod = getModWis();
            break;
    }


    SavingThrowRoll roll;
    roll.source = source;
    roll.defender = m_entity;
    roll.natural = m_level->random()->diceRoll(20);
    roll.modified = roll.natural + abilityScoreMod;

    applyAllModifiers( &roll );

    return roll;
}

int Actor::getModStr() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::STR ) );
}

int Actor::getModDex() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::DEX ) );
}

int Actor::getModCon() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::CON ) );
}

int Actor::getModInt() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::INT ) );
}

int Actor::getModWis() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::WIS ) );
}

int Actor::getModCha() const
{
    return abilityScoreToMod( getAbilityScoreValue( AbilityScoreType::CHA ) );
}

int Actor::getAbilityScoreValue(AbilityScoreType type) const
{
    switch(type)
    {
        case AbilityScoreType::STR:
            return m_baseAbilityScoreStr + getStaticModifier(ActorStaticModType::AttrStr);
        case AbilityScoreType::DEX:
            return m_baseAbilityScoreDex + getStaticModifier(ActorStaticModType::AttrDex);
        case AbilityScoreType::CON:
            return m_baseAbilityScoreCon + getStaticModifier(ActorStaticModType::AttrCon);
        case AbilityScoreType::INT:
            return m_baseAbilityScoreInt + getStaticModifier(ActorStaticModType::AttrInt);
        case AbilityScoreType::WIS:
            return m_baseAbilityScoreWis + getStaticModifier(ActorStaticModType::AttrWis);
        case AbilityScoreType::CHA:
            return m_baseAbilityScoreCha + getStaticModifier(ActorStaticModType::AttrCha);
    }
}

std::vector<GameAction> Actor::getAllGameActions() const
{
    std::vector<GameAction> out;
    out.reserve( m_actionMods.size() );
    
    for (auto const& mod : m_actionMods)
    {
        out.push_back( mod.action );
    }
    
    return out;
}

ActionsUsedInfo& Actor::actionInfo()
{
    return m_actionInfo;
}

ActionsUsedInfo const &Actor::actionInfo() const
{
    return m_actionInfo;
}

bool Actor::canPerformAction( GameAction const& action ) const
{
    ActionSpeedData speedData(&action);
    applyAllModifiers( &speedData );
    
    return actionInfo().canUseAction( speedData.modified );
}

MeleeAttackCountData Actor::getAttackCountForMeleeAttack( std::shared_ptr<MeleeAttack> attackImpl ) const
{
    MeleeAttackCountData data;
    auto [mainWeapon, offWeapon] = getEquippedWeapons();
    
    if ( attackImpl->isFullAttack() )
    {
        data.attacks.emplace_back( mainWeapon, 0 );
        
        if (offWeapon != nullptr)
        {
            data.attacks.emplace_back( offWeapon, 0 );
        }
    }
    else
    {
        data.attacks.emplace_back( mainWeapon, 0 );
    }
    
    applyAllModifiers(&data);
    
    return data;
}

std::optional<CreatureEquipSlot> Actor::canEquipItem( ItemPtr item )
{
    // For now, always return true.
    return defaultSlotForItemSlot( item->getEquipSlot() );
}

int Actor::getRefSave() const
{
    return m_baseReflex + getModDex() + getStaticModifier(ActorStaticModType::SaveRef);
}

int Actor::getWillSave() const
{
    return m_baseWill + getModWis() + getStaticModifier(ActorStaticModType::SaveWill);
}

int Actor::getFortSave() const
{
    return m_baseFortitude + getModCon() + getStaticModifier(ActorStaticModType::SaveFort);
}

int Actor::getStaticModifier( ActorStaticModType mod ) const
{
    int total = 0;
    auto range = m_staticModifiers.equal_range(mod);
    
    for ( auto it = range.first; it != range.second; it++ )
    {
        total += it->second.value;
    }
    
    return total;
}

int Actor::abilityScoreToMod(int score)
{
    return (score - 10) / 2;
}


ModifiableRollVisitor::ModifiableRollVisitor( Actor const* actor )
 : m_actor(actor) {}

void ModifiableRollVisitor::operator()( AttackRoll *roll )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::AttackRolls, roll );
}

void ModifiableRollVisitor::operator()( DamageRoll *roll )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::DamageRolls, roll );
}

void ModifiableRollVisitor::operator()( SavingThrowRoll *roll )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::SavingThrows, roll );
}

void ModifiableRollVisitor::operator()(AbilityScoreBonus *roll)
{
    m_actor->modifyTypedRoll(ActorDynamicModType::AbilityScores, roll );
}

void ModifiableRollVisitor::operator()( MovementSpeedData *roll )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::MovementSpeed, roll );
}

void ModifiableRollVisitor::operator()(ArmourClassData *data)
{
    m_actor->modifyTypedRoll(ActorDynamicModType::ArmourClassData, data );
}

void ModifiableRollVisitor::operator()( ActionSpeedData *data )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::ActionSpeedData, data );
}

void ModifiableRollVisitor::operator()( MeleeAttackCountData *data )
{
    m_actor->modifyTypedRoll(ActorDynamicModType::MeleeAttackCountData, data );
}
