#pragma once

#include <string>
#include <array>
#include <resource/Spritesheet.h>
#include <game/Items.h>
#include <game/Defines.h>

struct ItemData
{
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
    std::string itemName;
    DiceRoll damage;
    WeaponProficiency proficiency;
    int critLower;
    int critMult;
    std::string damageType;
    std::string specials;
    WeaponType weaponType;  // Ranged or Melee
};

struct ArmourData
{
    std::string itemName;
    int arcaneFailureChance;
    int armourBonus;
    int shieldBonus;
    int armourCheck;
    int maxDex;
    int speed20;
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

struct CreatureData
{
    std::string name;
    std::string description;

    int experience;
    int maxHP;
    DnDAlignment alignment;
    CreatureSize size;

    std::string creatureType;
    std::vector<std::string> creatureSubtypes;

    int saveFort;
    int saveRef;
    int saveWill;
    int attrStr;
    int attrDex;
    int attrCon;
    int attrInt;
    int attrWis;
    int attrCha;

    int reach;
    int initiative;

    std::unordered_map<std::string, int> damageResistance;
    std::unordered_map<ElementalDamageType, int> elementalResistance;
    std::vector<ElementalDamageType> weaknesses;
    std::vector<std::string> feats;
    std::vector<std::string> senses;
    std::vector<std::string> immune;

    std::vector<CreatureAttack> attacks;

    int speed;

    int combatManeuverDefence;
    int combatManeuverBonus;
    int baseAttackBonus;

    SpritesheetKey sprite;
};

struct RawPlayerRaceData
{
    std::string name;
    int baseSpeed;
};

struct RawPlayerClassData
{
    std::string name;
    std::string description;
    int hitDie;
    int skillsPerLevel;
    std::array<int, 20> babByLevel;
    std::array<int, 20> fortByLevel;
    std::array<int, 20> refByLevel;
    std::array<int, 20> willByLevel;
};

struct RawObjectData
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
    ActionSpeed speed;
    bool provokes;
    std::string description;
    SpritesheetKey sprite;
};

struct FeatData
{
    std::string id;
    std::string name;
    std::string benefit;
    std::string description;
};