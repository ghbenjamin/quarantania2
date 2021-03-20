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
    
    std::shared_ptr<Texture> getTexture();
    void setTexture( std::shared_ptr<Texture> tex );

private:
    std::vector<Tile> m_tiles;
    std::shared_ptr<Texture> m_texture;
};