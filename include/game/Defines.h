#pragma once

#include <iostream>

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

enum class AbilityScore
{
    STR,
    DEX,
    CON,
    INT,
    WIS,
    CHA
};

enum class Alignment
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
