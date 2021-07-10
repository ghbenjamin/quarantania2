#include <utils/GlobalConfig.h>
#include <utils/Json.h>

using namespace GlobalConfig;

GlobalConfigInfo GlobalConfig::load(std::string const &path)
{
    GlobalConfigInfo info;
    auto doc = Utils::Json::loadFromPath( path );

    info.windowTitle = doc.FindMember( "windowTitle" )->value.GetString();
    info.maxFPS = doc.FindMember( "maxFPS" )->value.GetInt();

    auto ssizeObj = doc.FindMember( "screenSize" )->value.GetObject();
    int w = ssizeObj.FindMember( "w" )->value.GetInt();
    int h = ssizeObj.FindMember( "h" )->value.GetInt();
    info.screenSize = {w, h};

    info.sizeToScreen = doc.FindMember( "sizeToScreen" )->value.GetBool();

    return info;
}
