#include <game/RunState.h>
#include <utils/Logging.h>

RunState::RunState()
{
    startTime = std::chrono::system_clock::now();
}

Utils::Json::Object RunState::serialize( Utils::Json::Document& doc ) const
{
    using namespace Utils::Json;

    Utils::Json::Object out;
    out.SetObject();

    Value seedVal;
    seedVal.SetString( this->seed.seed().c_str(), doc.GetAllocator() );
    out.AddMember( "seed", seedVal, doc.GetAllocator() );

    return out;
}
