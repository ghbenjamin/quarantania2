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

    void addRooms( int maxTries );
    void growMaze( Vector2i start );
    void connectRooms();
    void pruneCorridors();


    void tileSet(Vector2i tile, BaseTileType ttype);
    BaseTileType tileGet( Vector2i tile );

    Vector2i generateRandomRoomSize();

    void setDoor( Vector2i tile );

    bool canFloor( Vector2i coord, CardinalDirection dir );
    bool contains( Vector2i coord );
    int indexFromCoords( Vector2i coord );

private:

    std::random_device m_rd;
    std::mt19937 m_mt;

    BaseTileMap m_tilemap;
    Vector2i m_mapSize;
    std::vector<RectI> m_rooms;

    int m_regionIndex;
    std::unordered_map<Vector2i, int, Vector2Hash<int>> m_regionMap;
};