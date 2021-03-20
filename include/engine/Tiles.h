#pragma once

#include <memory>
#include <vector>

#include <resource/Spritesheet.h>


struct Tile
{
    Sprite sprite;
};

using TileRef = int;


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