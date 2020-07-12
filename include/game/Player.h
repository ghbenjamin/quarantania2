#pragma once

#include <string>
#include <memory>
#include <vector>

#include <engine/Entity.h>
#include <game/Defines.h>
#include <resource/Spritesheet.h>


// Complete description of an active player - this is the data which will be carried
// from level to level
struct PlayerData
{
    std::string name;
    std::string race;
    std::string clazz;
    Alignment alignment;
    SpritesheetKey sprite;

    int level;
    int currXP;

    int maxHP;
    int currHP;

    int attrStr;
    int attrDex;
    int attrCon;
    int attrInt;
    int attrWis;
    int attrCha;

    int saveFort;
    int saveRef;
    int saveWill;

    int baseSpeed;
    int bab;

    std::vector<std::string> startingHeldItems;
    std::vector<std::string> startingEquippedItems;
};



// Data given by the player in the level gen menu to construct a new starting player
struct PCData
{
    std::string name;
    std::string race;
    std::string clazz;
    Alignment alignment;
    SpritesheetKey sprite;

    int attrStr;
    int attrDex;
    int attrCon;
    int attrInt;
    int attrWis;
    int attrCha;

    PlayerData generateNewPlayer() const;
};

struct PartyData
{
    std::vector<PCData> playerChars;
};
