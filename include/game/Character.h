#pragma once

#include <unordered_map>


enum class Attribute
{
    AS_Str,
    AS_Dex,
    AS_Con,
    AS_Int,
    AS_Wis,
    AS_Cha,

    Save_Fort,
    Save_Will,
    Save_Ref,

    Def_AC,
    Def_AC_Flat,
    Def_CMD,
    Def_Spell_Resistance,

    Atk_BAB,
    Atk_CMB,

    Misc_Initiative,
    Misc_Speed,
    Misc_MaxDexAC,
};

enum class ModifierType
{
    Untyped,
};

struct ModifierData
{
    ModifierType type;
    int magnitude;

    bool operator==(const ModifierData &rhs) const;
};


class Character
{
public:

    Character();
    ~Character() = default;

    void addModifier(Attribute attr, ModifierData data);
    void removeModifier(Attribute attr, ModifierData data);
    int getBaseAttr(Attribute attr) const;
    void setBaseAttr(Attribute attr, int magnitude);

    int getStr() const;
    int getDex() const;
    int getCon() const;
    int getInt() const;
    int getWis() const;
    int getCha() const;

    int getStrMod() const;
    int getDexMod() const;
    int getConMod() const;
    int getIntMod() const;
    int getWisMod() const;
    int getChaMod() const;

    int getFortSave() const;
    int getRefSave() const;
    int getWillSave() const;

    int getBAB() const;
    int getCMD() const;
    int getCMB() const;

    int getAC() const;
    int getACFlat() const;

    int getInitiative() const;
    int getSpeed() const;
    int getSpellResist() const;

private:
    int getAttr(Attribute attr) const;
    void recache(Attribute attr);

private:
    std::unordered_map<Attribute, int> m_baseValues;
    std::unordered_map<Attribute, int> m_cachedValues;
    std::unordered_multimap<Attribute, ModifierData> m_modifiers;
};