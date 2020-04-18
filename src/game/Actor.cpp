#include <game/Actor.h>
#include <game/Player.h>
#include <db/RawData.h>
#include <fmt/format.h>
#include <utils/Assert.h>
#include <db/ResourceDatabase.h>
#include <game/Damage.h>
#include <utils/Logging.h>


Actor::Actor(const RawCreatureData& rcd)
{
    m_name = rcd.name;
    m_alignment = ResourceDatabase::parseAlignmentFromStr( rcd.alignment );

    m_abilityScores = {rcd.attrStr, rcd.attrDex, rcd.attrCon,
                       rcd.attrInt, rcd.attrWis, rcd.attrCha };
    m_baseFortSave = rcd.saveFort;
    m_baseRefSave = rcd.saveRef;
    m_baseWillSave = rcd.saveWill;
    m_baseBAB = rcd.bab;

    m_maxHP = rcd.hp;
    m_currentHP = m_maxHP;
    m_nonLethalDamage = 0;
}


Actor::Actor(PlayerData const& pdata)
{
    m_name = pdata.name;
    m_alignment = pdata.alignment;

    m_abilityScores = {pdata.attrStr, pdata.attrDex, pdata.attrCon,
                       pdata.attrInt, pdata.attrWis, pdata.attrCha };

    m_baseFortSave = pdata.saveFort;
    m_baseRefSave = pdata.saveRef;
    m_baseWillSave = pdata.saveWill;
    m_baseBAB = pdata.bab;

    m_maxHP = pdata.maxHP;
    m_currentHP = pdata.currHP;
    m_nonLethalDamage = 0;
}

std::string_view Actor::name() const
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
    if ( it != m_equippedItems.end() )
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

    m_equippedItems.emplace( slot, item );

    return lastEquipped;
}

WeaponPtr Actor::getActiveWeapon() const
{
    auto it = m_equippedItems.find( EquipSlot::Weapon );
    if ( it != m_equippedItems.end() )
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

int Actor::getModifier(ActorModifier mod) const
{
    auto it = m_modifiers.find(mod);

    if ( it != m_modifiers.end() )
    {
        return it->second.get();
    }
    else
    {
        return 0;
    }
}

void Actor::addModifier(ActorModifier mod, TypedModifier val)
{
    auto it = m_modifiers.find(mod);

    if ( it == m_modifiers.end() )
    {
        m_modifiers[mod] = ExModifierGroup();
    }

    m_modifiers[mod].addModifier( val );
}

void Actor::removeModifier(ActorModifier mod, TypedModifier val)
{
    auto it = m_modifiers.find(mod);
    Assert ( it != m_modifiers.end() );

    it->second.removeModifier( val );
}

int Actor::getStr() const
{
    return m_abilityScores.getScore( AbilityScore::STR ) + getModifier( ActorModifier::STR );
}

int Actor::getDex() const
{
    return m_abilityScores.getScore( AbilityScore::DEX ) + getModifier( ActorModifier::DEX );
}

int Actor::getCon() const
{
    return m_abilityScores.getScore( AbilityScore::CON ) + getModifier( ActorModifier::CON );
}

int Actor::getInt() const
{
    return m_abilityScores.getScore( AbilityScore::INT ) + getModifier( ActorModifier::INT );
}

int Actor::getWis() const
{
    return m_abilityScores.getScore( AbilityScore::WIS ) + getModifier( ActorModifier::WIS );
}

int Actor::getCha() const
{
    return m_abilityScores.getScore( AbilityScore::CHA ) + getModifier( ActorModifier::CHA );
}

int Actor::getStrMod() const
{
    return AbilityScoreBlock::getModifier( getStr() );
}

int Actor::getDexMod() const
{
    return AbilityScoreBlock::getModifier( getDex() );
}

int Actor::getConMod() const
{
    return AbilityScoreBlock::getModifier( getCon() );
}

int Actor::getIntMod() const
{
    return AbilityScoreBlock::getModifier( getInt() );
}

int Actor::getWisMod() const
{
    return AbilityScoreBlock::getModifier( getWis() );
}

int Actor::getChaMod() const
{
    return AbilityScoreBlock::getModifier( getCha() );
}

int Actor::getAC() const
{
    return 10 + getModifier( ActorModifier::AC ) + getDexMod(); // TODO Max dex modifier
}

int Actor::getTouchAC() const
{
    return 10 + getModifier( ActorModifier::TOUCH_AC ) + getDexMod();
}

int Actor::getFlatAC() const
{
    return 10 + getModifier( ActorModifier::FLAT_AC );
}

int Actor::getFortSave() const
{
    return m_baseFortSave + getModifier( ActorModifier::FORT ) + getConMod();
}

int Actor::getRefSave() const
{
    return m_baseRefSave + getModifier( ActorModifier::REFLEX ) + getDexMod();
}

int Actor::getWillSave() const
{
    return m_baseWillSave + getModifier( ActorModifier::WILL ) + getWisMod();
}

int Actor::getBAB() const
{
    return m_baseBAB + getModifier( ActorModifier::BAB );
}

int Actor::getCMD() const
{
    return getModifier( ActorModifier::CMD ) + getBAB() + getStrMod() + getDexMod() + 10;
}

int Actor::getCMB() const
{
    return getModifier( ActorModifier::CMB ) + getBAB() + getStrMod();
}

Alignment Actor::getAlignment() const
{
    return m_alignment;
}

bool Actor::isGood()
{
    return m_alignment == Alignment::LG || m_alignment == Alignment::NG || m_alignment == Alignment::CG;
}

bool Actor::isEvil()
{
    return m_alignment == Alignment::LE || m_alignment == Alignment::NE || m_alignment == Alignment::CE;
}

bool Actor::isLawful()
{
    return m_alignment == Alignment::LG || m_alignment == Alignment::LN || m_alignment == Alignment::LE;
}

bool Actor::isChaotic()
{
    return m_alignment == Alignment::CG || m_alignment == Alignment::CN || m_alignment == Alignment::CE;
}

bool Actor::isFlatFooted() const
{
    // TODO Implement
    return false;
}

AbilityScoreBlock::AbilityScoreBlock(int sstr, int sdex, int scon, int sint, int swis, int scha)
{
    m_scores = { sstr, sdex, scon, sint, swis, scha };
}

AbilityScoreBlock::AbilityScoreBlock()
{
    m_scores = { 10, 10, 10, 10, 10, 10 };
}

int AbilityScoreBlock::getScore(AbilityScore s) const
{
    return m_scores[ (int)s ];
}

void AbilityScoreBlock::setScore(AbilityScore s, int val)
{
    m_scores[ (int)s ] = val;
}

int AbilityScoreBlock::getModifier(int val)
{
    return (val - 10) / 2;
}

ExModifierGroup::ExModifierGroup()
: m_cached(0) {}

int ExModifierGroup::get() const
{
    return m_cached;
}

void ExModifierGroup::addModifier(TypedModifier tm)
{
    m_modifiers.push_back(tm);
    recache();
}

void ExModifierGroup::recache()
{
    std::unordered_map<ModifierType, int> totals;
    for ( auto const& tm : m_modifiers )
    {
        auto it = totals.find(tm.modType);
        if ( it == totals.end() )
        {
            totals[tm.modType] = tm.value;
        }
        else
        {
            totals[tm.modType] = std::max( tm.value, totals[tm.modType] );
        }
    }

    int total = 0;
    for ( auto const &[k, v] : totals )
    {
        total += v;
    }

    m_cached = total;
}

void ExModifierGroup::removeModifier(TypedModifier tm)
{
    auto it = std::find( m_modifiers.begin(), m_modifiers.end(), tm );
    if ( it != m_modifiers.end() )
    {
        m_modifiers.erase( it );
    }

    recache();
}

bool operator==(const TypedModifier &lhs, const TypedModifier &rhs)
{
    return lhs.modType == rhs.modType &&
           lhs.value == rhs.value;
}

bool operator!=(const TypedModifier &lhs, const TypedModifier &rhs)
{
    return !(rhs == lhs);
}

int Actor::acceptDamage( Damage const& dmg )
{
    int finalDamage = dmg.total;

    // TODO Resistances and immunities

    if ( finalDamage < 1 || dmg.type == DamageType::Nonlethal )
    {
        if ( finalDamage < 1 )
        {
            finalDamage = 1;
        }

        m_nonLethalDamage += finalDamage;

        if ( m_nonLethalDamage == m_currentHP )
        {
            // Staggered
        }
        else if ( m_nonLethalDamage > m_currentHP )
        {
            // Unconsious
        }
    }
    else
    {
        // Lethal damage

        m_currentHP -= finalDamage;

        if ( m_currentHP == 0 )
        {
            // Disabled
        }
        else if ( m_currentHP < 0 && m_currentHP > -getConMod() )
        {
            // Dying
        }
        else if ( m_currentHP < -getConMod() )
        {
            // Dead
        }
    }

    return finalDamage;
}