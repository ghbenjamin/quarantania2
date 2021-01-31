#include <game/Actor.h>
#include <game/Player.h>
#include <game/RawData.h>
#include <fmt/format.h>
#include <utils/Assert.h>
#include <game/ResourceDatabase.h>
#include <game/Damage.h>
#include <utils/Logging.h>



// Ability Score
// -------------------------------------------

AbilityScore::AbilityScore(int value)
{
    setValue(value);
}

void AbilityScore::setValue(int val)
{
    m_base = val;
    m_mod = val / 2;
}

int AbilityScore::getValue() const
{
    return m_base;
}

int AbilityScore::getMod() const
{
    return m_mod;
}


// Ability Score Block
// -------------------------------------------


AbilityScoreBlock::AbilityScoreBlock()
        : AbilityScoreBlock(0, 0, 0, 0, 0, 0) { }

AbilityScoreBlock::AbilityScoreBlock(int STR, int DEX, int CON, int INT, int WIS, int CHA)
{
    m_scores.emplace( AbilityScoreType::STR, STR );
    m_scores.emplace( AbilityScoreType::DEX, DEX );
    m_scores.emplace( AbilityScoreType::CON, CON );
    m_scores.emplace( AbilityScoreType::INT, INT );
    m_scores.emplace( AbilityScoreType::WIS, WIS );
    m_scores.emplace( AbilityScoreType::CHA, CHA );
}

AbilityScore const& AbilityScoreBlock::getScore(AbilityScoreType type) const
{
    return m_scores.at(type);
}


AbilityScore& AbilityScoreBlock::getScore(AbilityScoreType type)
{
    return m_scores.at(type);
}

// Actor
// ---------------------------------------------


Actor::Actor(CreatureData const& rcd)
    : m_name(rcd.name),
      m_abilityScores(rcd.attrStr, rcd.attrDex, rcd.attrCon,
      rcd.attrInt, rcd.attrWis, rcd.attrCha),
      m_HpMax(rcd.maxHP),
      m_HpCurrent(rcd.maxHP),
      m_baseSpeed(rcd.speed),
      m_size(rcd.size)
{
}

Actor::Actor(PlayerData const &pdata)
    : m_name(pdata.name),
      m_abilityScores(pdata.attrStr, pdata.attrDex, pdata.attrCon,
        pdata.attrInt, pdata.attrWis, pdata.attrCha),
      m_HpMax(pdata.maxHP),
      m_HpCurrent(pdata.maxHP),
      m_baseSpeed(pdata.baseSpeed),
      m_size(CreatureSize::Medium)
{
}

std::string const& Actor::getName() const
{
    return m_name;
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

Weapon const& Actor::getActiveWeapon() const
{
    if ( hasEquipped( CreatureEquipSlot::RightHand ) )
    {
        auto right = getEquipped( CreatureEquipSlot::RightHand );
        if ( right->getEquipSlot() == ItemEquipSlot::Weapon )
        {
            return right->getWeapon();
        }
    }

    else if ( hasEquipped( CreatureEquipSlot::LeftHand ) )
    {
        auto left = getEquipped( CreatureEquipSlot::LeftHand );
        if ( left->getEquipSlot() == ItemEquipSlot::Weapon )
        {
            return left->getWeapon();
        }
    }

    return getNaturalWeapon();
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

AbilityScoreBlock &Actor::abilityScores()
{
    return m_abilityScores;
}

int Actor::getCurrentHp() const
{
    return m_HpCurrent;
}

int Actor::getMaxHp() const
{
    return m_HpMax;
}

void Actor::setCurrentHp(int value)
{
    m_HpCurrent = value;
}

int Actor::getAC() const
{
    int dexToAc = m_abilityScores.getScore( AbilityScoreType::DEX ).getMod();
    int armourAC = 0;
    int shieldAC = 0;

    if ( hasEquipped(CreatureEquipSlot::Armour) )
    {
        auto const& armour = getEquipped( CreatureEquipSlot::Armour )->getArmour();

        dexToAc = std::min( dexToAc, armour.maxDex() );
        armourAC = armour.armourBonus();
    }

    auto* shield = tryGetActiveShield();

    if ( shield != nullptr )
    {
        dexToAc = std::min( dexToAc, shield->maxDex() );
        shieldAC = shield->armourBonus();
    }

    return dexToAc + armourAC + shieldAC + 10;
}

int Actor::getSpeed() const
{
    return m_baseSpeed;
}

CreatureSize Actor::getSize()
{
    return m_size;
}

float Actor::getReach() const
{
    return 1.5f;
}

std::optional<CreatureEquipSlot> Actor::defaultSlotForItemSlot(ItemEquipSlot slot) const
{
    switch( slot )
    {
        case ItemEquipSlot::Armor:
            return CreatureEquipSlot::Armour;
            break;
        case ItemEquipSlot::Arms:
            return CreatureEquipSlot::Arms;
            break;
        case ItemEquipSlot::Belt:
            return CreatureEquipSlot::Belt;
            break;
        case ItemEquipSlot::Body:
            return CreatureEquipSlot::Body;
            break;
        case ItemEquipSlot::Chest:
            return CreatureEquipSlot::Chest;
            break;
        case ItemEquipSlot::Eyes:
            return CreatureEquipSlot::Eyes;
            break;
        case ItemEquipSlot::Feet:
            return CreatureEquipSlot::Feet;
            break;
        case ItemEquipSlot::Hands:
            return CreatureEquipSlot::RightHand;
            break;
        case ItemEquipSlot::Head:
            return CreatureEquipSlot::Head;
            break;
        case ItemEquipSlot::Headband:
            return CreatureEquipSlot::Headband;
            break;
        case ItemEquipSlot::Neck:
            return CreatureEquipSlot::Neck;
            break;
        case ItemEquipSlot::Ring:
            return CreatureEquipSlot::Ring1;
            break;
        case ItemEquipSlot::Shield:
            return CreatureEquipSlot::LeftHand;
            break;
        case ItemEquipSlot::Shoulders:
            return CreatureEquipSlot::Shoulders;
            break;
        case ItemEquipSlot::Weapon:
            return CreatureEquipSlot::RightHand;
            break;
        case ItemEquipSlot::Wrists:
            return CreatureEquipSlot::Wrists;
            break;
        default:
            return {};
            break;
    }
}

std::unordered_map<CreatureEquipSlot, ItemPtr> const &Actor::getAllEquippedItems() const
{
    return m_equippedItems;
}

