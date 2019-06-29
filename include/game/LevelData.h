#pragma once

#include <utils/Containers.h>
#include <game/Tiles.h>


namespace LD
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

enum class JunctionType
{
    Open,
    Door
};

using RegionRef = int;

struct Junction
{
    Vector2i pos;
    JunctionType type;

    RegionRef region1 = -1;
    RegionRef region2 = -1;
};

struct Room
{
    RectI bounds;
    std::vector<Vector2i> junctions;
};

struct LevelTransition
{
    Vector2i pos;
    RegionRef containingRegion = -1;
};

using BaseTileMap = std::vector<BaseTileType>;

}

struct ImmutableLevelData
{
    ImmutableLevelData() = default;
    ~ImmutableLevelData() = default;

    // Core information
    Vector2i levelSize;
    int tilePixelSize = -1;
    int tileCount = -1;

    // Rendering information
    TileRenderMap renderTileMap;
    std::vector<std::vector<TileRef>> mapRendering;

    // Map information
    LD::BaseTileMap baseTilemap;

    // Game logic
    Vector2i entranceTile;
    Vector2i exitTile;

    // Game actors

};