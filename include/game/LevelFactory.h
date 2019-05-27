#pragma once

#include <random>

#include <game/Level.h>
#include <game/LevelConfig.h>
#include <utils/Grid.h>


enum class BaseTileType
{
    Entrance,
    Exit,
    Wall,
    Floor,
    Door
};

using BaseTileMap = std::vector<BaseTileType>;

class LevelFactory
{
public:

    LevelFactory();
    ~LevelFactory() = default;

    LevelPtr create( LevelConfig const& config, LevelContextPtr const& ctx );

private:
    BaseTileMap generateLayout(  LevelConfig const& config, LevelContextPtr const& ctx );
    void growMaze( Vector2i start );

    void tileSet(Vector2i tile, BaseTileType ttype);
    BaseTileType tileGet( Vector2i tile );

    bool canFloor( Vector2i coord, CardinalDirection dir );
    bool contains( Vector2i coord );
    int indexFromCoords( Vector2i coord );

private:

    std::random_device m_rd;
    std::mt19937 m_mt;

    BaseTileMap m_tilemap;
    Vector2i m_mapSize;
};