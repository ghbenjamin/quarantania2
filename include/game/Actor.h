#pragma once

#include <unordered_map>
#include <string_view>
#include <array>

#include <game/Items.h>

struct RawCreatureData;
struct PlayerData;


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


class AbilityScoreBlock
{
public:
    AbilityScoreBlock(int, int, int, int, int, int);
    AbilityScoreBlock();
    ~AbilityScoreBlock() = default;

    int getScore( AbilityScore s ) const;
    void setScore( AbilityScore s, int val );

    static inline int getModifier( int val );

private:
    std::array<int, 6> m_scores;
};


class Actor
{
public:

    Actor(PlayerData const& pdata);
    Actor(RawCreatureData const& rcd);
    ~Actor() = default;

    // Modifiers
    int getModifier(ActorModifier mod) const;
    void addModifier(ActorModifier mod, TypedModifier val);
    void removeModifier(ActorModifier mod, TypedModifier val);

    // Info
    std::string_view name() const;

    // Alignment
    Alignment getAlignment() const;
    bool isGood();
    bool isEvil();
    bool isLawful();
    bool isChaotic();

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

    // Modifiers
    std::unordered_map<ActorModifier, ExModifierGroup> m_modifiers;

    // Info
    std::string m_name;
    Alignment m_alignment;

    // Stats
    AbilityScoreBlock m_abilityScores;
    int m_baseFortSave;
    int m_baseRefSave;
    int m_baseWillSave;
    int m_baseBAB;

    // Health
    int m_maxHP;
    int m_currentHP;
    int m_nonLethalHP;

    // Items
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
    std::vector<WeaponPtr> m_naturalWeapons;

};