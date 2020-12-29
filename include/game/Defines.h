#pragma once

#include <iostream>

// Level Properties
// --------------------------------------


enum class Passibility
{
    Passable,
    Difficult,
    Impassable
};

enum class Visibility
{
    Hidden,
    Explored,
    Visible
};

using LightLevel = std::uint8_t;



// Creature Properties
// --------------------------------------

enum class AbilityScoreType
{
    STR,
    DEX,
    CON,
    INT,
    WIS,
    CHA
};

enum class DnDAlignment
{
    LG,
    NG,
    CG,
    LN,
    TN,
    CN,
    LE,
    NE,
    CE
};

enum class CreatureSize
{
    Tiny,
    Small,
    Medium,
    Large,
    Huge,
    Gargantuan,
    Colossal
};


// Item Properties
// --------------------------------------


enum class ItemType
{
    Unknown,
    Armour,
    Consumable,
    Equippable,
    Gear,
    Weapon,
    Ammo,
};

enum class EquipSlot
{
    None,
    Armor,
    Arms,
    Belt,
    Body,
    Chest,
    Eyes,
    Feet,
    Hands,
    Head,
    Headband,
    Neck,
    Ring,
    Shield,
    Shoulders,
    Weapon,
    Wrists,
};


enum class WeaponHandedness
{
    OneHanded,
    TwoHanded,
};

enum class WeaponType
{
    Melee,
    Ranged,
    Ammo
};

enum class WeaponProficiency
{
    Simple,
    Martial,
    Exotic
};

enum class PhysDamageType
{
    Piercing    = 0x1,
    Bludgeoning = 0x2,
    Slashing    = 0x4
};

enum class WeaponSpecials
{
    Trip        = 0x01,
    Reach       = 0x02,
    Nonlethal   = 0x04,
    Double      = 0x08,
    Disarm      = 0x10,
    Brace       = 0x20,
};

using PhysDamageMask = std::uint8_t;
using WeaponSpecialMask = std::uint8_t;

enum class ArmourType
{
    Heavy,
    Medium,
    Light,
    Shield
};



// Damage Properties
// --------------------------------------

enum class DamageType
{
    Untyped,
    Elemental,
    Force,
    Energy,
    Alignment,
    Poison,
    Mental,
    Nonlethal,
    Precision
};

enum class ElementalDamageType
{
    Fire,
    Cold,
    Electricity,
    Acid
};

enum class DamageSuperType
{
    Magical,
    Physical
};



// Misc Properties
// --------------------------------------

struct DiceRoll
{
    int diceCount;
    int diceSize;
};

struct CritData
{
    int lowerRange;
    int multiplier;
};