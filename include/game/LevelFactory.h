#pragma once

#include <random>
#include <map>

#include <game/Level.h>
#include <game/LevelData.h>
#include <game/LevelConfig.h>
#include <utils/GridUtils.h>
#include <resource/RoomTemplates.h>


class LevelFactory
{
public:

    LevelFactory();
    ~LevelFactory() = default;

    /**
     * Create a single level from the given configuration and context.
     * Place rooms, connect them with corridors and generate doors. Fill the rooms with
     * stuff. Generate other NPCs. Add appropriately placed entrances and exits.
     * @param config
     * @param ctx
     * @return
     */
    LevelPtr create( LevelConfig const& config, LevelContextPtr const& ctx );

private:

    /**
     *  Basic level layout construction: placement of rooms, corridors, doors.
     */

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
    void tileSet(Vector2i tile, LD::BaseTileType ttype);

    // Return the type of the specified tile
    LD::BaseTileType tileGet( Vector2i tile );

    // Generate a random-ish room size. Sizes are guaranteed to be odd.
    Vector2i generateRandomRoomSize();

    // Add a junction between two regions - normally a door
    void addJunction(Vector2i pos, LD::RegionRef r1, LD::RegionRef r2);
    void removeJunction(Vector2i pos);

    // Can the floor tile at this position be extended in the given direction without breaking into
    // a different region?
    bool canFloor( Vector2i coord, Direction dir );

    // Convert from a 2x position to a 1d index
    int indexFromCoords( Vector2i coord );

    // Convert from a 1d index to a 2d position
    Vector2i coordsFromIndex( int idx );

    // Generate a bitmask indicating which of the 8 surrounding tiles contain a wall
    GridBitmask adjacentWalls( Vector2i coord );
    void calcAllAdjacentWalls();

    // Start a new region of the given type
    void newRegion( LD::RegionType type );

    // Try to generate the entrances and exits
    void generateEntrancesExits();

    void readyAllEntities();

    void setInitialCollisionData();

    /**
     *  Fill the level with actual stuff: entity placement, room decoration.
     */

    void decorateRooms();
    void constructPlayer();
    void constructDoors();


private:
    LevelPtr m_level;

    RoomTemplateManager m_roomTemplates;

    // Random
    std::random_device m_rd;
    std::mt19937 m_mt;

    // Base map layout
    int m_regionIndex;

    // Map region -> rooms
    std::map<LD::RegionRef, LD::Room> m_rooms;

    // Tile position -> containing region
    std::unordered_map<Vector2i, LD::RegionRef, Vector2Hash<int>> m_regionMap;

    // Map junction position -> junction data
    std::unordered_map<Vector2i, LD::Junction, Vector2Hash<int>> m_junctions;

    // Map region id -> connected junctions
    std::unordered_map<LD::RegionRef, std::vector<Vector2i>> m_regionToJunctions;

    // Map region -> type of region eg. door, corridor
    std::unordered_map<LD::RegionRef, LD::RegionType> m_regionTypeMap;

    // Flattened 1d array of positions of walls surrounding each tile
    std::vector<GridBitmask> m_wallPositionMasks;

    // All the entities which we create while generating the level
    std::vector<EntityRef> m_createdEntities;
};