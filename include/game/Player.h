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

    PlayerRace playerRace;
    PlayerClass playerClass;
    DnDAlignment alignment;
    SpritesheetKey sprite;

    int level = 1;
    int currXP = 0;

    int maxHP;
    int currHP;

    int attrStr = 10;
    int attrDex = 10;
    int attrCon = 10;
    int attrInt = 10;
    int attrWis = 10;
    int attrCha = 10;

    int saveFort = 0;
    int saveRef = 0;
    int saveWill = 0;

    int baseSpeed = 6;
    int bab = 0;

    std::vector<std::string> startingHeldItems;
    std::vector<std::string> startingEquippedItems;
};


struct PartyData
{
    std::vector<PlayerData> playerChars;
};
