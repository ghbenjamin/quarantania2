#pragma once

#include <array>

#include <game/Action.h>
#include <engine/Component.h>
#include <game/Defines.h>
#include <game/Items.h>

struct RawCreatureData;
struct PlayerData;
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


enum class ActorType
{
    PC,
    NPC
};

struct ActorComponent : public Component<ActorComponent>
{
    ActorComponent( PlayerData const& pdata );
    ActorComponent( RawCreatureData const& data );
    ~ActorComponent() override = default;

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


    static constexpr int modifierFromVal( int val );

    ///////////////////////////////////////


    // Modifiers
    std::unordered_map<ActorModifier, ExModifierGroup> modifiers;

    // Info
    const std::string name;
    Alignment alignment;
    ActorType actorType;

    // Stats
    int baseSTR;
    int baseDEX;
    int baseCON;
    int baseINT;
    int baseWIS;
    int baseCHA;
    int baseFortSave;
    int baseRefSave;
    int baseWillSave;
    int baseBAB;

    // Health
    int maxHP;
    int currentHP;
    int nonLethalDamage;
    int temporaryHP;

    // Items
    std::unordered_map<EquipSlot, ItemPtr> equippedItems;
    std::vector<WeaponPtr> naturalWeapons;

    // Actions
    int initiative;
    int energyPerTick;
    int currentEnergy;
    int maxEnergy;

    ActionPtr nextAction;
};
