#pragma once

#include <chrono>

#include <utils/Random.h>
#include <utils/Json.h>
#include <engine/Entity.h>
#include <game/Defines.h>
#include <game/ActorData.h>
#include <resource/Spritesheet.h>

// Forward definitions
class Overworld;

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
    std::shared_ptr<Overworld> overworld;

    // Current party state
    std::vector<PlayerData> playerChars;

private:
    RunState();
    PlayerData generateStartingCharacter( std::string const& pclassId );
};
