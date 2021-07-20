#pragma once

#include <chrono>

#include <utils/Random.h>
#include <utils/Json.h>
#include <engine/Entity.h>
#include <game/Defines.h>
#include <resource/Spritesheet.h>

struct ChargenData;

struct PlayerData
{
    PlayerData() = default;
    PlayerData( ChargenData& chargenData );
    
    utils::json::object serialize() const;
    
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

    std::vector<std::string> heldItems;
    std::vector<std::string> equippedItems;
    std::vector<std::string> featIds;
};


class RunState
{
public:
    
    static std::shared_ptr<RunState> generateNewRun();
    
    ~RunState() = default;

    utils::json::object serialize() const;

    // Globals
    std::chrono::time_point<std::chrono::system_clock> startTime;
    RandomState randomState;
    
    // Current overworld state
    int currentLocation;
    std::vector<int> locationsVisited;

    // Current party state
    std::vector<PlayerData> playerChars;

private:
    RunState();
    PlayerData generateStartingCharacter( std::string const& pclassId );
};
