#pragma once

#include <chrono>

#include <utils/Random.h>
#include <utils/Json.h>
#include <engine/Entity.h>
#include <game/Defines.h>
#include <resource/Spritesheet.h>


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
    std::vector<std::string> featIds;
};


class RunState
{
public:

    RunState();
    ~RunState() = default;

    nlohmann::json serialize() const;

    // Globals
    std::chrono::time_point<std::chrono::system_clock> startTime;
    RandomSeed seed;

    // Current overworld state
    int currentLocation;
    std::vector<int> locationsVisited;

    // Current party state
    std::vector<PlayerData> playerChars;
};