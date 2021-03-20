#pragma once

#include <utils/Containers.h>
#include <engine/Tiles.h>


enum class BaseTileType
{
    Wall,
    Floor,
    Junction
};


struct LevelLayout
{
    Tileset tileset;
    std::vector<TileRef> mapData;
    std::vector<BaseTileType> tileTypes;
};