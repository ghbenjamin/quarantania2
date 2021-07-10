#include <game/RunState.h>
#include <utils/Logging.h>

RunState::RunState()
{
    startTime = std::chrono::system_clock::now();
}

nlohmann::json RunState::serialize( ) const
{
    nlohmann::json json;

    return json;
}
