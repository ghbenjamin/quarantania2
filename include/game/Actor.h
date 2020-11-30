#pragma once


#include <game/Defines.h>
#include <game/Items.h>
#include <game/Action.h>

struct RawCreatureData;
struct PlayerData;
struct Damage;


// Things which can be dynamically altered about an actor
enum class ActorModifier
{
    STR,
    DEX,
    CON,
    INT,
    WIS,
    CHA,

    FORT,
    REFLEX,
    WILL,

    AC,
    TOUCH_AC,
    FLAT_AC,
    AC_MAX_DEX_MOD,

    CMB,
    CMD,
    BAB,
};

enum class ModifierType
{
    Untyped
};

struct TypedModifier
{
    ModifierType modType;
    int value;

    friend bool operator==(const TypedModifier &lhs, const TypedModifier &rhs);
    friend bool operator!=(const TypedModifier &lhs, const TypedModifier &rhs);
};

// Exclusive modifier group
class ExModifierGroup
{
public:
    ExModifierGroup();
    ~ExModifierGroup() = default;

    int get() const;
    void addModifier(TypedModifier tm);
    void removeModifier(TypedModifier tm);

private:
    void recache();

    int m_cached;
    std::vector<TypedModifier> m_modifiers;
};



class Actor
{

public:

    Actor( PlayerData const& pdata );
    Actor( RawCreatureData const& data );
    ~Actor() = default;

    // Meta
    std::string const& getName();

    // Modifiers
    int getModifier(ActorModifier mod) const;
    void addModifier(ActorModifier mod, TypedModifier val);
    void removeModifier(ActorModifier mod, TypedModifier val);

    // Alignment
    bool isGood() const;
    bool isEvil() const;
    bool isLawful() const;
    bool isChaotic() const;

    // Stats
    int getStr() const;
    int getStrMod() const;
    int getDex() const;
    int getDexMod() const;
    int getCon() const;
    int getConMod() const;
    int getInt() const;
    int getIntMod() const;
    int getWis() const;
    int getWisMod() const;
    int getCha() const;
    int getChaMod() const;
    int getIthAbilityScore(int i) const;
    int getIthAbilityScoreMod(int i) const;

    // Saves
    int getFortSave() const;
    int getRefSave() const;
    int getWillSave() const;

    // Armour
    int getAC() const;
    int getTouchAC() const;
    int getFlatAC() const;

    // Combat
    int getBAB() const;
    int getCMD() const;
    int getCMB() const;
    bool isFlatFooted() const;

    // Items
    bool hasEquipped( EquipSlot slot ) const;
    const ItemPtr getEquipped( EquipSlot slot ) const;
    ItemPtr unequipItem( EquipSlot slot );
    ItemPtr equipItem( EquipSlot slot, ItemPtr item );

    // Weapons
    WeaponPtr getActiveWeapon() const;
    WeaponPtr getNaturalWeapon() const;


private:
    static constexpr int modifierFromVal( int val );


    // Modifiers
    std::unordered_map<ActorModifier, ExModifierGroup> m_modifiers;

    // Info
    const std::string m_name;
    Alignment m_alignment;

    // Stats
    int m_baseSTR;
    int m_baseDEX;
    int m_baseCON;
    int m_baseINT;
    int m_baseWIS;
    int m_baseCHA;

    int m_baseFortSave;
    int m_baseRefSave;
    int m_baseWillSave;
    int m_baseBAB;

    // Health
    int m_maxHP;
    int m_currentHP;


    // Items
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
    std::vector<WeaponPtr> m_naturalWeapons;

    // Actions
    int initiative;
};