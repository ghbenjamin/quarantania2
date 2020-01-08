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


class TileRenderMap
{
public:
    TileRenderMap() = default;
    ~TileRenderMap() = default;

    TileRef addTile(TerrainTile ttype, const SpritesheetKey &key);
    Tile const& get( TileRef ref ) const;
    Tile const& get( TerrainTile ttype ) const;
    TileRef getRef( TerrainTile ttype ) const;

private:
    std::vector<Tile> m_tiles;
    std::unordered_map<TerrainTile, TileRef> m_names;
};