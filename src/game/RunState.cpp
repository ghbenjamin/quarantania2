#include <game/RunState.h>



RunState::RunState()
{
    startTime = std::chrono::time_point_cast<std::chrono::milliseconds>( std::chrono::system_clock::now() );
    currentLocation = 0;
}

nlohmann::json RunState::serialize( ) const
{
    nlohmann::json json;

    json["meta"]["startTime"] = this->startTime.time_since_epoch().count();
    json["meta"]["seed"] = this->seed.seed();

    json["overworld"]["currentLoc"] = this->currentLocation;
    json["overworld"]["locsVisited"] = this->locationsVisited;

    return json;
}
