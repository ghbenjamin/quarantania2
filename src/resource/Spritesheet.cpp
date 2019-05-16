#include <resource/Spritesheet.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

Spritesheet::Spritesheet(TexturePtr texture, int margin, int tileSize)
: m_texture(texture), m_margin(margin), m_tileSize(tileSize)
{
    auto w = m_texture->size().x();
    w -= tileSize;
    m_tileWidth = ( w / (margin + tileSize) ) + 1;
}

const RectI Spritesheet::getRegion(int id) const
{
    return RectI{
        { (id % m_tileWidth) * (m_tileSize + m_margin), (id / m_tileWidth) * (m_tileSize + m_margin) },
        { m_tileSize, m_tileSize }
    };
}

const Vector2i Spritesheet::sheetPosFromGid(int gid) const
{
    int x = (gid % m_tileWidth) * (m_tileSize + m_margin);
    int y = (gid / m_tileWidth) * (m_tileSize + m_margin);

    return {x, y};
}

Sprite Spritesheet::spriteFromGid(int gid)
{
    auto sheetPos = sheetPosFromGid( gid );
    return Sprite {
        m_texture,
        { sheetPos, { m_tileSize, m_tileSize } }
    };
}

Sprite Spritesheet::spriteFromName(std::string const &name)
{
    return Sprite();
}
