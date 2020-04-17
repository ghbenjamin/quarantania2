#pragma once

#include <string>
#include <array>
#include <resource/Spritesheet.h>
#include <game/Items.h>
#include <game/Defines.h>

struct RawItemData
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

struct RawWeaponData
{
    std::string itemName;
    std::string weaponClass;
    DiceRoll damage;
    WeaponProficiency proficiency;
    int critLower;
    int critMult;
    std::string damageType;
    std::string specials;
};

struct RawArmourData
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

struct RawCreatureData
{
    std::string name;
    std::string description;

    int xp;
    int hp;
    std::string alignment;
    std::string size;

    std::string creatureType;
    std::string creatureSubtype;

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
    std::string damageResist;
    std::string feats;
    std::string immune;
    std::string languages;
    std::string resist;
    std::string senses;

    int speedLand;
    int speedFly;
    int speedSwim;

    int cmd;
    int cmb;
    int bab;

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