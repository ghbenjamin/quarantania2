#pragma once

#include <string>
#include <utils/Containers.h>

namespace GlobalConfig
{

struct GlobalConfigInfo
{
    std::string windowTitle;
    Vector2i screenSize;
    int maxFPS;
};

GlobalConfigInfo load( std::string const& path );

}