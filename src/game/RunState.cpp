#include <game/RunState.h>
#include <game/ResourceDatabase.h>
#include <utils/Logging.h>


PlayerData::PlayerData( ChargenData &chargenData )
{
    this->sprite = chargenData.sprite;
    
    this->maxHP = chargenData.maxHP;
    
    this->attrStr = chargenData.attrStr;
    this->attrDex = chargenData.attrDex;
    this->attrCon = chargenData.attrCon;
    this->attrInt = chargenData.attrInt;
    this->attrWis = chargenData.attrWis;
    this->attrCha = chargenData.attrCha;
    
    this->featIds = chargenData.feats;
    this->equippedItems = chargenData.equippedItems;
    this->heldItems = chargenData.heldItems;
}

utils::json::object PlayerData::serialize() const
{
    utils::json::object obj;
    
    obj["name"] = name;
    obj["curr_xp"] = currHP;
    obj["max_hp"] = maxHP;
    obj["curr_hp"] = currHP;
    
    obj["attributes"] = { attrStr, attrDex, attrCon, attrInt, attrWis, attrCha };
    obj["saves"] = { saveFort, saveRef, saveWill };
    obj["bab"] = bab;
    
    obj["held_items"] = utils::json::object::array();
    for ( auto const& i : heldItems )
    {
        obj["held_items"].push_back(i);
    }
    
    obj["equipped_items"] = utils::json::object::array();
    for ( auto const& i : equippedItems )
    {
        obj["equipped_items"].push_back(i);
    }
    
    obj["feats"] = utils::json::object::array();
    for ( auto const& i : featIds )
    {
        obj["feats"].push_back(i);
    }
    
    return obj;
}


RunState::RunState()
{
    startTime = std::chrono::time_point_cast<std::chrono::milliseconds>( std::chrono::system_clock::now() );
    currentLocation = 0;
}

utils::json::object RunState::serialize( ) const
{
    utils::json::object json;

    json["meta"]["startTime"] = this->startTime.time_since_epoch().count();
    json["meta"]["seed"] = this->randomState.seed();
    json["meta"]["prngState"] = this->randomState.prngState();

    json["overworld"]["currentLoc"] = this->currentLocation;
    json["overworld"]["locsVisited"] = this->locationsVisited;
    
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

    auto p1 = state->generateStartingCharacter("fighter");
    auto p2 = state->generateStartingCharacter("rogue");
    state->playerChars = { p1, p2 };
    
    return state;
}

PlayerData RunState::generateStartingCharacter( std::string const &pclassId )
{
    auto pcData = ResourceDatabase::instance().chargenFromClassId(pclassId);
    PlayerData player { pcData };
    
    player.name = *randomState.randomElement( ResourceDatabase::instance().randomNames() );
    
    return player;
}

