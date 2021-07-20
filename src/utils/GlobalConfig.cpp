#include <utils/GlobalConfig.h>
#include <utils/Json.h>

using namespace GlobalConfig;

GlobalConfigInfo GlobalConfig::load(std::string const &path)
{
    GlobalConfigInfo info;
    auto doc = utils::json::loadFromPath(path );

    info.windowTitle = doc["windowTitle"];
    info.maxFPS = doc["maxFPS"];

    auto ssize = doc["screenSize"];
    info.screenSize = { ssize["w"], ssize["h"] };

    info.sizeToScreen = doc["sizeToScreen"];

    return info;
}
