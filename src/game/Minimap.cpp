#include <game/Minimap.h>
#include <resource/Texture.h>
#include <utils/Ranges.h>
#include <utils/Logging.h>


Minimap::Minimap(Vector2i size, int scale)
: m_size(size),
  m_scale(scale),
  m_data(0),
  m_byteWidth(-1)
{
    // (px_x, px_y) = (tile_x, tile_y) * pixels per tile
    Vector2i pixelSize = size * scale;
    int pixelCount = pixelSize.area();

    m_texture = std::make_shared<Texture>(pixelSize);
    m_byteWidth = m_texture->size().x() * 4;
    m_data.reserve( pixelCount * 4 );

    // Default to all black
    for (int i = 0; i < pixelCount; i++)
    {
        m_data.push_back( static_cast<std::byte>(0) );
        m_data.push_back( static_cast<std::byte>(0) );
        m_data.push_back( static_cast<std::byte>(200) );
        m_data.push_back( static_cast<std::byte>(255) );
    }

    updateTexture();
}

void Minimap::updateTexture()
{
    SDL_UpdateTexture(
        m_texture->raw(),
        NULL,
        &m_data[0],
        m_byteWidth
    );
}

void Minimap::setTile(int tileIdx, Colour const &colour)
{
    setTile({ tileIdx % m_size.x(), tileIdx / m_size.x() }, colour);
}

void Minimap::setTile(Vector2i coord, Colour const &colour)
{
    Vector2i scaled = coord * m_scale;

    for ( auto const it : RectIRange({scaled.x(), scaled.y(), m_scale, m_scale}) )
    {
        setPixel(it, colour);
    }
}

void Minimap::setPixelRegion(RectI area, Colour const &colour)
{
    for ( auto const& it : RectIRange{area} )
    {
        setPixel(it, colour);
    }
}

std::shared_ptr<Texture> Minimap::getTexture() const
{
    return m_texture;
}

void Minimap::setPixel(int pixelIdx, Colour const &colour)
{
    std::size_t byteIdx = pixelIdx * 4;

    m_data[ byteIdx     ] = static_cast<std::byte> ( colour.getR() );
    m_data[ byteIdx + 1 ] = static_cast<std::byte> ( colour.getG() );
    m_data[ byteIdx + 2 ] = static_cast<std::byte> ( colour.getB() );
    m_data[ byteIdx + 3 ] = static_cast<std::byte> ( colour.getA() );
}

void Minimap::setPixel(Vector2i pixelCoord, Colour const &colour)
{
    int pixelIdx = pixelCoord.x() + pixelCoord.y() * m_size.x() * m_scale;
    setPixel(pixelIdx, colour);
}
