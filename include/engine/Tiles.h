#pragma once

#include <memory>
#include <vector>

#include <resource/Spritesheet.h>


struct Tile
{
    Sprite sprite;
};

using TileRef = int;


enum class TerrainTile
{
    Wall_Center,
    Wall_Horizontal,
    Wall_Vertical,
    Wall_Corner_SE,
    Wall_Corner_SW,
    Wall_Corner_NE,
    Wall_Corner_NW,
    Wall_Cross,
    Wall_T_N,
    Wall_T_E,
    Wall_T_S,
    Wall_T_W,

    Floor
};


class Tileset
{
public:
    Tileset() = default;
    ~Tileset() = default;

    TileRef addTile(SpritesheetKey const& key);
    Tile& get( TileRef ref );

private:
    std::vector<Tile> m_tiles;
};