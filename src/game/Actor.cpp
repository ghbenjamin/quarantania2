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

AbilityScore &AbilityScoreBlock::getScore(AbilityScoreType type)
{
    return m_scores.at(type);
}



// Actor
// ---------------------------------------------


Actor::Actor(RawCreatureData const& rcd)
    : m_name(rcd.name),
      m_abilityScores(rcd.attrStr, rcd.attrDex, rcd.attrCon, rcd.attrInt, rcd.attrWis, rcd.attrCha)
{
}

Actor::Actor(PlayerData const &pdata)
    : m_name(pdata.name),
      m_abilityScores(pdata.attrStr, pdata.attrDex, pdata.attrCon, pdata.attrInt, pdata.attrWis, pdata.attrCha)
{
}

std::string const& Actor::getName() const
{
    return m_name;
}

bool Actor::hasEquipped(EquipSlot slot) const
{
    return m_equippedItems.find(slot) != m_equippedItems.end();
}

const ItemPtr Actor::getEquipped(EquipSlot slot) const
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

ItemPtr Actor::unequipItem(EquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

ItemPtr Actor::equipItem(EquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    m_equippedItems.emplace(slot, item );

    return lastEquipped;
}

WeaponPtr Actor::getActiveWeapon() const
{
    auto it = m_equippedItems.find(EquipSlot::Weapon);
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

WeaponPtr Actor::getNaturalWeapon() const
{
    if ( m_naturalWeapons.empty() )
    {
        return WeaponPtr();
    }

    // TODO Don't just return the front weapon
    return m_naturalWeapons.front();
}

AbilityScoreBlock &Actor::abilityScores()
{
    return m_abilityScores;
}

bool Actor::hasWeapon() const
{
    return !!getActiveWeapon();
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
