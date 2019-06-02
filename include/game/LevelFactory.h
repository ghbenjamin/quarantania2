#pragma once

#include <random>
#include <map>

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
        std::vector<Vector2i> junctions;
    };

    using BaseTileMap = std::vector<BaseTileType>;
}


class LevelFactory
{
public:

    LevelFactory();
    ~LevelFactory() = default;

    /**
     * Create a single level from the given configuration and context.
     * Place rooms, connect them with corridors and generate doors. Fill the rooms with
     * stuff. Generate other NPCs. Add appropriately places entrances and exits.
     * @param config
     * @param ctx
     * @return
     */
    LevelPtr create( LevelConfig const& config, LevelContextPtr const& ctx );

private:

    // Level rendering methods

    // Walk over the level as it currently exists, and generate + place the correct sprites to render
    // the level.
    void constructMapRendering(LevelConfig const &config, LevelContextPtr const &ctx);

    // Which wall sprite (straight vs corner vs T-junction etc.) should be placed at the given
    // position given the wall tiles that surround it if any?
    TileRef getCorrectWallTile( int idx );

    // Base level layout methods

    // Attempt to place a random number of rooms. maxTries determines how sparsely packed the room will be:
    // high maxTries -> densely packed rooms
    void addRooms( int maxTries );

    // Fill all of the non-room portions of the level with simple random mazes
    void fillAllMazes();

    // Connect up the rooms and corridors by adding doors such that all the rooms and corridors are connected
    void connectRooms();

    // Get rid of all of the corridors which don't go anywhere
    void pruneCorridors();

    // Grow a random maze from a given starting position, filling as much space as is possible. These mazes are locked
    // to the odd-spaced tiles only so that proper walls
    void growMaze( Vector2i start );

    // Set the specified tile to the specified tile type, keeping the regions up to date if necessary
    void tileSet(Vector2i tile, LF::BaseTileType ttype);

    // Return the type of the specified tile
    LF::BaseTileType tileGet( Vector2i tile );

    // Generate a random-ish room size. Sizes are guaranteed to be odd.
    Vector2i generateRandomRoomSize();

    // Add a junction between two regions - normally a door
    void addJunction(LF::Junction jc);

    // Can the floor tile at this position be extended in the given direction without breaking into
    // a different region?
    bool canFloor( Vector2i coord, Direction dir );

    // Does our grid contain the given position?
    bool gridContains(Vector2i coord);


    // Convert from a 2x position to a 1d index
    int indexFromCoords( Vector2i coord );

    // Convert from a 1d index to a 2d position
    Vector2i coordsFromIndex( int idx );

    // Generate a bitmask indicating which of the 8 surrounding tiles contain a wall
    GridBitmask adjacentWalls( Vector2i coord );
    void calcAllAdjacentWalls();

    void newRegion( LF::RegionType type );

    void generateEntrancesExits();

private:

    // Main data structure
    ImmutableLevelData m_imdata;

    // Random
    std::random_device m_rd;
    std::mt19937 m_mt;

    // Rendering


    // Base map layout
    LF::BaseTileMap m_tilemap;
    std::map<int, LF::Room> m_rooms;
    int m_regionIndex;
    std::unordered_map<Vector2i, int, Vector2Hash<int>> m_regionMap;
    std::unordered_map<Vector2i, LF::Junction, Vector2Hash<int>> m_junctions;
    std::unordered_map<int, LF::RegionType> m_regionTypeMap;
    std::vector<GridBitmask> m_wallPositionMasks;
};