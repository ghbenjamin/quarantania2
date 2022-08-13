#include <utils/GlobalConfig.h>
#include <sol/sol.hpp>
#include <engine/LuaState.h>

using namespace GlobalConfig;

GlobalConfigInfo GlobalConfig::load(std::string const &path, LuaState& lua)
{
    lua.runScriptFile( path );

    GlobalConfigInfo info;
    auto const& luaConf = lua.state()["config"];

    info.windowTitle = luaConf["windowTitle"].get<sol::string_view>();;
    info.maxFPS = luaConf["maxFPS"];

    auto const& ssize = luaConf["screenSize"];
    info.screenSize = { ssize["w"], ssize["h"] };

    info.sizeToScreen = luaConf["sizeToScreen"];

    return info;
}
