#pragma once

#include <game/Level.h>
#include <game/LevelConfig.h>

namespace LevelFactory
{
    LevelPtr create( LevelConfig const& config, LevelContextPtr const& ctx );
}