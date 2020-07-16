#pragma once

#include <utils/Containers.h>
#include <engine/Tiles.h>


namespace LD
{

enum class BaseTileType
{
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

enum class RoomType
{
    Normal,

    Entrance,
    Exit,
    Shop,
    Boss,
    Vault,
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
    RoomType roomType = RoomType::Normal;
    std::vector<Vector2i> junctions;

    Vector2i centre() const;
};

using BaseTileMap = std::vector<BaseTileType>;


struct LevelLayout
{
    TileRenderMap renderTilemap;
    std::vector<TileRef> mapData;
    LD::BaseTileMap baseTilemap;
};

}