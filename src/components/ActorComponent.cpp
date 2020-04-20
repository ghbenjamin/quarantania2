#include <components/ActorComponent.h>
#include <game/Player.h>
#include <db/RawData.h>
#include <fmt/format.h>
#include <utils/Assert.h>
#include <db/ResourceDatabase.h>
#include <game/Damage.h>
#include <utils/Logging.h>


ActorComponent::ActorComponent(RawCreatureData const& rcd)
: initiative(200), energyPerTick(10), currentEnergy(0), maxEnergy(10),
  name(rcd.name)
{
    alignment = ResourceDatabase::parseAlignmentFromStr( rcd.alignment );

    baseSTR = rcd.attrStr;
    baseDEX = rcd.attrDex;
    baseCON = rcd.attrCon;
    baseINT = rcd.attrInt;
    baseWIS = rcd.attrWis;
    baseCHA = rcd.attrCha;
    baseFortSave = rcd.saveFort;
    baseRefSave = rcd.saveRef;
    baseWillSave = rcd.saveWill;
    baseBAB = rcd.bab;

    maxHP = rcd.hp;
    currentHP = maxHP;
    nonLethalDamage = 0;
}

ActorComponent::ActorComponent(PlayerData const &pdata)
: initiative(200), energyPerTick(10), currentEnergy(0), maxEnergy(10),
    name(pdata.name)
{
    alignment = pdata.alignment;
    baseSTR = pdata.attrStr;
    baseDEX = pdata.attrDex;
    baseCON = pdata.attrCon;
    baseINT = pdata.attrInt;
    baseWIS = pdata.attrWis;
    baseCHA = pdata.attrCha;
    baseFortSave = pdata.saveFort;
    baseRefSave = pdata.saveRef;
    baseWillSave = pdata.saveWill;
    baseBAB = pdata.bab;

    maxHP = pdata.maxHP;
    currentHP = pdata.currHP;
    nonLethalDamage = 0;
}

bool ActorComponent::hasEquipped(EquipSlot slot) const
{
    return equippedItems.find(slot) != equippedItems.end();
}

const ItemPtr ActorComponent::getEquipped(EquipSlot slot) const
{
    auto it = equippedItems.find(slot);
    if (it != equippedItems.end() )
    {
        return it->second;
    }
    else
    {
        return ItemPtr();
    }
}

ItemPtr ActorComponent::unequipItem(EquipSlot slot)
{
    auto it = equippedItems.find(slot);
    auto ptr = it->second;

    equippedItems.erase(it);
    return ptr;
}

ItemPtr ActorComponent::equipItem(EquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    equippedItems.emplace( slot, item );

    return lastEquipped;
}

WeaponPtr ActorComponent::getActiveWeapon() const
{
    auto it = equippedItems.find( EquipSlot::Weapon );
    if (it != equippedItems.end() )
    {
        return it->second->getWeapon();
    }
    else
    {
        // Might return a null ptr
        return getNaturalWeapon();
    }
}

WeaponPtr ActorComponent::getNaturalWeapon() const
{
    if ( naturalWeapons.empty() )
    {
        return WeaponPtr();
    }

    // TODO Don't just return the front weapon
    return naturalWeapons.front();
}

int ActorComponent::getModifier(ActorModifier mod) const
{
    auto it = modifiers.find(mod);

    if (it != modifiers.end() )
    {
        return it->second.get();
    }
    else
    {
        return 0;
    }
}

void ActorComponent::addModifier(ActorModifier mod, TypedModifier val)
{
    auto it = modifiers.find(mod);

    if (it == modifiers.end() )
    {
        modifiers[mod] = ExModifierGroup();
    }

    modifiers[mod].addModifier( val );
}

void ActorComponent::removeModifier(ActorModifier mod, TypedModifier val)
{
    auto it = modifiers.find(mod);
    Assert (it != modifiers.end() );

    it->second.removeModifier( val );
}

int ActorComponent::getStr() const
{
    return baseSTR + getModifier( ActorModifier::STR );
}

int ActorComponent::getDex() const
{
    return baseDEX + getModifier( ActorModifier::DEX );
}

int ActorComponent::getCon() const
{
    return baseCON + getModifier( ActorModifier::CON );
}

int ActorComponent::getInt() const
{
    return baseINT + getModifier( ActorModifier::INT );
}

int ActorComponent::getWis() const
{
    return baseWIS + getModifier( ActorModifier::WIS );
}

int ActorComponent::getCha() const
{
    return baseCHA + getModifier( ActorModifier::CHA );
}

int ActorComponent::getStrMod() const
{
    return ActorComponent::modifierFromVal( getStr() );
}

int ActorComponent::getDexMod() const
{
    return ActorComponent::modifierFromVal( getDex() );
}

int ActorComponent::getConMod() const
{
    return ActorComponent::modifierFromVal( getCon() );
}

int ActorComponent::getIntMod() const
{
    return ActorComponent::modifierFromVal( getInt() );
}

int ActorComponent::getWisMod() const
{
    return ActorComponent::modifierFromVal( getWis() );
}

int ActorComponent::getChaMod() const
{
    return ActorComponent::modifierFromVal( getCha() );
}

int ActorComponent::getAC() const
{
    return 10 + getModifier( ActorModifier::AC ) + getDexMod(); // TODO Max dex modifier
}

int ActorComponent::getTouchAC() const
{
    return 10 + getModifier( ActorModifier::TOUCH_AC ) + getDexMod();
}

int ActorComponent::getFlatAC() const
{
    return 10 + getModifier( ActorModifier::FLAT_AC );
}

int ActorComponent::getFortSave() const
{
    return baseFortSave + getModifier( ActorModifier::FORT ) + getConMod();
}

int ActorComponent::getRefSave() const
{
    return baseRefSave + getModifier( ActorModifier::REFLEX ) + getDexMod();
}

int ActorComponent::getWillSave() const
{
    return baseWillSave + getModifier( ActorModifier::WILL ) + getWisMod();
}

int ActorComponent::getBAB() const
{
    return baseBAB + getModifier( ActorModifier::BAB );
}

int ActorComponent::getCMD() const
{
    return getModifier( ActorModifier::CMD ) + getBAB() + getStrMod() + getDexMod() + 10;
}

int ActorComponent::getCMB() const
{
    return getModifier( ActorModifier::CMB ) + getBAB() + getStrMod();
}


bool ActorComponent::isGood() const
{
    return alignment == Alignment::LG || alignment == Alignment::NG || alignment == Alignment::CG;
}

bool ActorComponent::isEvil() const
{
    return alignment == Alignment::LE || alignment == Alignment::NE || alignment == Alignment::CE;
}

bool ActorComponent::isLawful() const
{
    return alignment == Alignment::LG || alignment == Alignment::LN || alignment == Alignment::LE;
}

bool ActorComponent::isChaotic() const
{
    return alignment == Alignment::CG || alignment == Alignment::CN || alignment == Alignment::CE;
}

bool ActorComponent::isFlatFooted() const
{
    // TODO Implement
    return false;
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

constexpr int ActorComponent::modifierFromVal(int val)
{
    return (val - 10) / 2;
}

int ActorComponent::getIthAbilityScore(int i) const
{
    switch (i)
    {
        case 0:
            return getStr();
        case 1:
            return getDex();
        case 2:
            return getCon();
        case 3:
            return getInt();
        case 4:
            return getWis();
        case 5:
            return getCha();
        default:
            AssertAlways();
            return -1;
    }
}

int ActorComponent::getIthAbilityScoreMod(int i) const
{
    return ActorComponent::modifierFromVal( getIthAbilityScore(i) );
}
