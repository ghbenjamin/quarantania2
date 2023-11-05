#pragma once

#include <string>
#include <array>
#include <resource/Spritesheet.h>
#include <game/Items.h>
#include <game/Defines.h>

struct ItemData
{
    std::string id;
    std::string name;
    int weight;
    int value;
    std::string description;
    std::string auraValue;
    std::string auraStrength;
    std::string slot;
    std::string itemType;

    SpritesheetKey sprite;
};

struct WeaponData
{
    std::string itemId;
    DiceRoll damage;
    int critLower;
    int critMult;
    WeaponType weaponType;  // Ranged or Melee
};

struct ArmourData
{
    std::string itemId;
    int arcaneFailureChance;
    int armourBonus;
    int shieldBonus;
    int armourCheck;
    int maxDex;
    int speed30;
    std::string armourType;
};

struct CreatureAttack
{
    std::string name;
    DiceRoll stats;
    int toHit;
    int count;
};


struct ObjectData
{
    std::string name;
    std::string description;
    std::string type;
    std::vector<SpritesheetKey> sprites;
};


enum class RawActionDataType
{
    Move,
    Attack,
    Item
};

struct ActionData
{
    std::string id;
    std::string name;
    RawActionDataType type;
    int speed;
    bool provokes;
    std::string description;
    SpritesheetKey sprite;
};

struct ModifierData
{
    std::string id;
    std::string name;
    std::string effect;
    std::string description;
};
