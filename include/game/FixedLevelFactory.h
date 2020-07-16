#pragma once

#include <game/Level.h>
#include <game/LevelData.h>
#include <game/LevelConfig.h>

struct TiledMap;

class FixedLevelFactory
{
public:

    FixedLevelFactory();
    ~FixedLevelFactory() = default;

    LevelPtr create(TiledMap const& map, LevelContextPtr const& ctx, PartyData const& pdata);
};