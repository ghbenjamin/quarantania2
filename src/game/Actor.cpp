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

bool Actor::hasEquipped(ItemEquipSlot slot) const
{
    return m_equippedItems.find(slot) != m_equippedItems.end();
}

const ItemPtr Actor::getEquipped(ItemEquipSlot slot) const
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

ItemPtr Actor::unequipItem(ItemEquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

ItemPtr Actor::equipItem(ItemEquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    m_equippedItems.emplace(slot, item );

    return lastEquipped;
}

Weapon const& Actor::getActiveWeapon() const
{
    auto it = m_equippedItems.find(ItemEquipSlot::Weapon);
    if (it != m_equippedItems.end() )
    {
        return it->second->getWeapon();
    }
    else
    {
        // Might return a null ptr
        return getNaturalWeapon();
    }
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

    if ( hasEquipped(ItemEquipSlot::Armor) )
    {
        auto const& armour = getEquipped( ItemEquipSlot::Armor )->getArmour();

        dexToAc = std::min( dexToAc, armour.maxDex() );
        armourAC = armour.armourBonus();
    }

    if ( hasEquipped(ItemEquipSlot::Shield) )
    {
        auto const& shield = getEquipped( ItemEquipSlot::Armor )->getArmour();

        dexToAc = std::min( dexToAc, shield.maxDex() );
        shieldAC = shield.armourBonus();
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
