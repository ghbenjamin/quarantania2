#pragma once

#include <string>
#include <utils/Containers.h>

class LuaState;

namespace GlobalConfig
{

struct GlobalConfigInfo
{
    std::string windowTitle;
    bool sizeToScreen;
    Vector2i screenSize;
    int maxFPS;
};

GlobalConfigInfo load( std::string const& path, LuaState& lua );

static constexpr int TileSizePx = 32;
static constexpr Vector2i TileDimsPx = {TileSizePx, TileSizePx};

}