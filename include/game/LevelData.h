#pragma once

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

using RegionRef = int;

struct Junction
{
    Vector2i pos;
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
    int tilePixelSize = -1;
    Vector2i levelSize;
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