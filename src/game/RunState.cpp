#include <game/RunState.h>
#include <game/ResourceDatabase.h>
#include <game/Overworld.h>
#include <game/OverworldFactory.h>


RunState::RunState()
{
    startTime = std::chrono::time_point_cast<std::chrono::milliseconds>( std::chrono::system_clock::now() );
}

utils::json::object RunState::serialize( ) const
{
    utils::json::object json;

    json["meta"]["startTime"] = this->startTime.time_since_epoch().count();
    json["meta"]["seed"] = this->randomState.seed();
    json["meta"]["prngState"] = this->randomState.prngState();

    json["overworld"]["currentLoc"] = this->overworld->currentLocation();
//    json["overworld"]["locsVisited"] = this->overworld->;
    
    json["party"] = utils::json::object::array();
    for ( auto const& p : playerChars )
    {
        json["party"].push_back( p.serialize() );
    }
    
//    Logging::log( json.dump(1) );
    return json;
}

std::shared_ptr<RunState> RunState::generateNewRun()
{
    auto state = std::shared_ptr<RunState>( new RunState() );
    
    OverworldFactory factory;
    state->overworld = factory.createOverworld( state );

    auto p1 = state->generateStartingCharacter("Fighter");
    auto p2 = state->generateStartingCharacter("Rogue");
    state->playerChars = { p1, p2 };
    
    return state;
}

PlayerData RunState::generateStartingCharacter( std::string const &pclassId )
{
    auto pcData = ResourceDatabase::get().chargenFromClass(pclassId);
    PlayerData player { pcData };
    
    player.name = *randomState.randomElement( ResourceDatabase::get().randomNames() );
    
    return player;
}

