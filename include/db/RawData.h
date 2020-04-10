#pragma once

#include <string>
#include <resource/Spritesheet.h>

struct RawItemData
{
    std::string name;
    int weight;
    int value;
    std::string description;
    std::string aura_value;
    std::string aura_strength;
    std::string slot;
    std::string item_type;

    SpritesheetKey sprite;
};

struct RawWeaponData
{
    std::string item_name;
    std::string weapon_class;
    std::string damage;
    std::string proficiency;
    int crit_lower;
    int crit_mult;
    std::string damage_type;
    std::string specials;
};

struct RawArmourData
{
    std::string item_name;
    int arcaneFailureChance;
    int armourBonus;
    int shieldBonus;
    int armourCheck;
    int maxDex;
    int speed20;
    int speed30;
    std::string armour_type;
};

struct RawCreatureData
{
    std::string name;
    std::string description;

    int xp;
    int hp;
    std::string alignment;
    std::string size;

    std::string creature_type;
    std::string creature_subtype;

    int save_fort;
    int save_ref;
    int save_will;
    int attr_str;
    int attr_dex;
    int attr_con;
    int attr_int;
    int attr_wis;
    int attr_cha;

    int reach;
    std::string damageResist;
    std::string feats;
    std::string immune;
    std::string languages;
    std::string resist;
    std::string senses;

    int speed_land;
    int speed_fly;
    int speed_swim;

    int cmd;
    int cmb;
    int bab;

    SpritesheetKey sprite;
};
