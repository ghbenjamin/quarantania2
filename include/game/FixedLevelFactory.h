#pragma once

#include <game/Level.h>
#include <game/LevelData.h>
#include <game/LevelConfig.h>

class FixedLevelFactory
{
public:

    FixedLevelFactory();
    ~FixedLevelFactory() = default;

    LevelPtr create(FixedLevelConfig const& config, LevelContextPtr const& ctx, PartyData const& pdata);
};