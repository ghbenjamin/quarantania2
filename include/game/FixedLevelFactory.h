#pragma once

#include <game/LevelFactory.h>

struct TiledMap;
struct TiledObjectLayer;

class FixedLevelFactory : public LevelFactory
{
public:

    FixedLevelFactory();
    ~FixedLevelFactory() = default;

    LevelPtr create(TiledMap const& map, LevelContextPtr const& ctx, PartyData const& pdata);


private:

    void assembleTiledMap(TiledMap const& map);
    void constructFixedObjects(TiledObjectLayer const& olayer);

};