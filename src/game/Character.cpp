#include <game/Character.h>
#include <algorithm>

Character::Character()
{
    m_baseValues[Attribute::AS_Str] = 10;
    m_baseValues[Attribute::AS_Dex] = 10;
    m_baseValues[Attribute::AS_Con] = 10;
    m_baseValues[Attribute::AS_Int] = 10;
    m_baseValues[Attribute::AS_Wis] = 10;
    m_baseValues[Attribute::AS_Cha] = 10;
    m_baseValues[Attribute::Save_Fort] = 0;
    m_baseValues[Attribute::Save_Will] = 0;
    m_baseValues[Attribute::Save_Ref] = 0;
    m_baseValues[Attribute::Def_AC] = 0;
    m_baseValues[Attribute::Def_AC_Flat] = 0;
    m_baseValues[Attribute::Def_CMD] = 0;
    m_baseValues[Attribute::Def_Spell_Resistance] = 0;
    m_baseValues[Attribute::Atk_BAB] = 0;
    m_baseValues[Attribute::Atk_CMB] = 0;
    m_baseValues[Attribute::Misc_Initiative] = 0;
    m_baseValues[Attribute::Misc_Speed] = 0;
    m_baseValues[Attribute::Misc_MaxDexAC] = std::numeric_limits<int>::max();

    for ( auto const& [k, v] : m_baseValues )
    {
        m_cachedValues[k] = v;
    }
}

void Character::addModifier(Attribute attr, ModifierData data)
{
    m_modifiers.emplace(attr, data);
}

void Character::removeModifier(Attribute attr, ModifierData data)
{
    auto range = m_modifiers.equal_range(attr);

    for ( auto it = range.first; it != range.second; it++ )
    {
        if ( it->second == data )
        {
            m_modifiers.erase(it);
            break;
        }
    }
}

int Character::getBaseAttr(Attribute attr) const
{
    return m_baseValues.at(attr);
}

void Character::setBaseAttr(Attribute attr, int magnitude)
{
    m_baseValues[attr] = magnitude;
    recache(attr);
}

int Character::getAttr(Attribute attr) const
{
    return m_cachedValues.at(attr);
}

void Character::recache(Attribute attr)
{
    auto eq = m_modifiers.equal_range(attr);
    std::unordered_map<ModifierType, int> buff;

    for (auto it = eq.first; it != eq.second; it++)
    {
        ModifierType mt = it->second.type;
        auto mod_it = buff.find(mt);
        if ( mod_it == buff.end() )
        {
            buff[mt] = it->second.magnitude;
        }
        else
        {
            buff[mt] = std::max( buff[mt], it->second.magnitude );
        }
    }

    int cached = m_baseValues[attr];
    for ( auto [k, v] : buff )
    {
        cached += v;
    }

    m_cachedValues[attr] = cached;
}

int Character::getStr() const
{
    return m_cachedValues.at(Attribute::AS_Str);
}

int Character::getDex() const
{
    return m_cachedValues.at(Attribute::AS_Dex);
}

int Character::getCon() const
{
    return m_cachedValues.at(Attribute::AS_Con);
}

int Character::getInt() const
{
    return m_cachedValues.at(Attribute::AS_Int);
}

int Character::getWis() const
{
    return m_cachedValues.at(Attribute::AS_Wis);
}

int Character::getCha() const
{
    return m_cachedValues.at(Attribute::AS_Cha);
}

int Character::getStrMod() const
{
    return (getStr() - 10) / 2;
}

int Character::getDexMod() const
{
    return (getDex() - 10) / 2;
}

int Character::getConMod() const
{
    return (getCon() - 10) / 2;
}

int Character::getIntMod() const
{
    return (getInt() - 10) / 2;
}

int Character::getWisMod() const
{
    return (getWis() - 10) / 2;
}

int Character::getChaMod() const
{
    return (getCha() - 10) / 2;
}

int Character::getFortSave() const
{
    return getAttr(Attribute::Save_Fort) + getConMod();
}

int Character::getRefSave() const
{
    return getAttr(Attribute::Save_Ref) + getDexMod();
}

int Character::getWillSave() const
{
    return getAttr(Attribute::Save_Will) + getWisMod();
}

int Character::getBAB() const
{
    return getAttr(Attribute::Atk_BAB);
}

int Character::getCMD() const
{
    return getAttr(Attribute::Def_CMD) + getBAB() + getStrMod() + 10;
}

int Character::getCMB() const
{
    return getAttr(Attribute::Atk_CMB) + getBAB() + getStrMod();
}

int Character::getAC() const
{
    return getAttr(Attribute::Def_AC)
        + 10
        + std::min(
            getDexMod(),
            getAttr(Attribute::Misc_MaxDexAC)
          );
}

int Character::getACFlat() const
{
    return 10 + getAttr(Attribute::Def_AC) + getAttr(Attribute::Def_AC_Flat);
}

int Character::getInitiative() const
{
    return getAttr(Attribute::Misc_Initiative) + getDexMod();
}

int Character::getSpeed() const
{
    return getAttr(Attribute::Misc_Speed);
}

int Character::getSpellResist() const
{
    return getAttr(Attribute::Def_Spell_Resistance);
}

bool ModifierData::operator==(const ModifierData &rhs) const
{
    return type == rhs.type &&
           magnitude == rhs.magnitude;
}