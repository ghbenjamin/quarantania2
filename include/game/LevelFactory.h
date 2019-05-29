#pragma once

#include <random>

#include <game/Level.h>
#include <game/LevelConfig.h>
#include <utils/Grid.h>

namespace LF
{
    enum class BaseTileType
    {
        Entrance,
        Exit,
        Wall,
        Floor,
        Door
    };

    enum class RegionType
    {
        Room,
        Corridor
    };

    struct Door
    {
        Vector2i pos;
        int region1 = -1;
        int region2 = -1;
    };

    struct Room
    {
        RectI bounds;
        std::vector<Door> doors;
    };

    using BaseTileMap = std::vector<BaseTileType>;
}


class LevelFactory
{
public:

    LevelFactory();
    ~LevelFactory() = default;

    LevelPtr create( LevelConfig const& config, LevelContextPtr const& ctx );

private:

    // Level rendering methods

    void constructTileMap( LevelConfig const& config, LevelContextPtr const& ctx );


    // Base level layout methods

    LF::BaseTileMap generateLayout(  LevelConfig const& config, LevelContextPtr const& ctx );

    void addRooms( int maxTries );
    void fillAllMazes();
    void connectRooms();
    void pruneCorridors();

    void growMaze( Vector2i start );

    void tileSet(Vector2i tile, LF::BaseTileType ttype);
    LF::BaseTileType tileGet( Vector2i tile );

    Vector2i generateRandomRoomSize();

    void setDoor( LF::Door tile );

    bool canFloor( Vector2i coord, CardinalDirection dir );
    bool contains( Vector2i coord );
    int indexFromCoords( Vector2i coord );

    void newRegion( LF::RegionType type );

private:

    // Main data structure
    ImmutableLevelData m_imdata;

    // Random
    std::random_device m_rd;
    std::mt19937 m_mt;

    // Rendering


    // Base map layout
    LF::BaseTileMap m_tilemap;
    Vector2i m_mapSize;
    std::vector<LF::Room> m_rooms;
    int m_regionIndex;
    std::unordered_map<Vector2i, int, Vector2Hash<int>> m_regionMap;
    std::unordered_map<Vector2i, LF::Door, Vector2Hash<int>> m_doors;
    std::unordered_map<int, LF::RegionType> m_regionTypeMap;

};