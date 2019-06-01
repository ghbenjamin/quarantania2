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
        Junction
    };

    enum class RegionType
    {
        Room,
        Corridor
    };

    struct Junction
    {
        Vector2i pos;
        int region1 = -1;
        int region2 = -1;
    };

    struct Room
    {
        RectI bounds;
        std::vector<Junction> junctions;
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

    void constructMapRendering(LevelConfig const &config, LevelContextPtr const &ctx);
    TileRef getCorrectWallTile( Vector2i tile );

    // Base level layout methods

    void addRooms( int maxTries );
    void fillAllMazes();
    void connectRooms();
    void pruneCorridors();

    void growMaze( Vector2i start );

    void tileSet(Vector2i tile, LF::BaseTileType ttype);
    LF::BaseTileType tileGet( Vector2i tile );

    Vector2i generateRandomRoomSize();

    void addJunction(LF::Junction jc);

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
    std::vector<LF::Room> m_rooms;
    int m_regionIndex;
    std::unordered_map<Vector2i, int, Vector2Hash<int>> m_regionMap;
    std::unordered_map<Vector2i, LF::Junction, Vector2Hash<int>> m_junctions;
    std::unordered_map<int, LF::RegionType> m_regionTypeMap;

};