#pragma once

#include <memory>
#include <vector>

#include <resource/Spritesheet.h>


struct Tile
{
    Sprite sprite;
    bool passible = true;
};

using TileRef = int;


class TileRenderMap
{
public:
    TileRenderMap() = default;
    ~TileRenderMap() = default;

    TileRef addTile( const SpritesheetKey& key, bool passible );

    Tile const& get( TileRef ref ) const;
    Tile const& get( std::string const& name ) const;
    TileRef getRef( std::string const& name ) const;

private:
    std::vector<Tile> m_tiles;
    std::unordered_map<std::string, TileRef> m_names;
};