#pragma once

#include <utils/Containers.h>
#include <utils/Colour.h>

class Texture;

class Minimap
{
public:
    Minimap(Vector2i size, int scale);
    ~Minimap() = default;


    void setTile(Vector2i coord, Colour const& colour );
    void setTile(int tileIdx, Colour const& colour );

    void movePlayer( Vector2i tileCoord );

    void updateTexture();

    std::shared_ptr<Texture> getTexture() const;

private:

    void setPixel(int pixelIdx, Colour const& colour);
    void setPixel(Vector2i pixelCoord, Colour const& colour);
    void setPixelRegion(RectI area, Colour const& colour );

    inline std::size_t tileCoordToByteOffset(Vector2i coord) const;

    // Size of the minimap in *tiles*
    const Vector2i m_size;

    // Ratio of tiles to minimal pixels
    const int m_scale;

    // The number of bytes per row of minimap pixels
    int m_byteWidth;

    // Texture data as bytes and as an SDL_Texture*
    std::vector<std::byte> m_data;
    std::shared_ptr<Texture> m_texture;

    Vector2i m_playerLocation;
    Colour const& m_playerHighlightColour;
    std::vector<std::byte> m_playerExcluding;
};