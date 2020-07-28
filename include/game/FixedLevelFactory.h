#pragma once

#include <game/LevelFactory.h>

struct TiledMap;
struct TiledObjectLayer;

class FixedLevelFactory : public LevelFactory
{
public:

    FixedLevelFactory();
    ~FixedLevelFactory() override = default;

    LevelPtr create(TiledMap const* map, LevelContextPtr const& ctx, PartyData const& pdata);


private:

    void constructTiles();
    void constructObjects();

    void constructFixedObjects(TiledObjectLayer const& olayer);
    void constructDecor(TiledObjectLayer const& olayer);
    void constructSpawnPoints(TiledObjectLayer const& olayer);

    void constructParty(PartyData const& pdata);

    std::vector<Vector2i> m_playerSpawns;

    TiledMap const* m_map;
};