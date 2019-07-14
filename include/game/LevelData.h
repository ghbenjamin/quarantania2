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
    JunctionType type = JunctionType::Door;

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