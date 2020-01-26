#include <game/UISystem.h>
#include <resource/Texture.h>
#include <utils/Ranges.h>
#include <utils/Logging.h>


Minimap::Minimap(Vector2i size, int scale)
: m_size(size),
  m_scale(scale),
  m_data(0),
  m_byteWidth(-1),
  m_playerExcluding(m_scale * m_scale * 4, std::byte{0}),
  m_playerLocation({-1, -1}),
  m_playerHighlightColour( Colour::Red )
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

void Minimap::movePlayer(Vector2i tileCoord)
{
    // Store the colour data for the tile the player is about to move to in a buffer and then colour the new tile
    // appropriately. Restore the data at the previous player location from the old buffer.
    // NB. this is not 100% necessary at the moment since all player movement is completely regenerating the minimap
    // Hopefully in the future this will not be the case, so this method will be a useful performance saver.

    Vector2i oldLoc = m_playerLocation;
    m_playerLocation = tileCoord;

    if ( oldLoc != Vector2i{-1, -1})
    {
        // We had an old player position - put the tile the player is standing on back
        std::copy(
            m_playerExcluding.begin(),
            m_playerExcluding.end(),
            m_data.begin() + tileCoordToByteOffset(oldLoc)
        );
    }

    std::size_t offset = tileCoordToByteOffset(m_playerLocation);

    // Buffer the data in the tile the player is now standing on
    std::copy(
        m_data.begin() + offset,
        m_data.begin() + offset + m_playerExcluding.size(),
        m_playerExcluding.begin()
    );

    setTile(m_playerLocation, m_playerHighlightColour);
}

std::size_t Minimap::tileCoordToByteOffset(Vector2i coord) const
{
    return coord.x() * m_scale * 4
         + coord.y() * m_size.x() * m_scale * 4;
}
