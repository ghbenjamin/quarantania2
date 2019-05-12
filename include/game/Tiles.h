#pragma once

#include <memory>
#include <vector>


struct TileInfo
{

};



class Tile
{
public:
private:
};



class TileMap
{
public:
    TileMap() = default;
    ~TileMap() = default;

private:
    int m_nextTileId;
    std::vector<Tile> m_tileLookup;

};

