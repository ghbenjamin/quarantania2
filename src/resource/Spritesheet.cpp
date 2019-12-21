#include <resource/Spritesheet.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

Spritesheet::Spritesheet(TexturePtr texture)
: m_texture(texture)
{

}

TiledSpritesheet::TiledSpritesheet(TexturePtr texture, SpritesheetGidMap& gidMap, int margin, int tileSize)
: Spritesheet(texture), m_margin(margin), m_tileSize(tileSize)
{
    auto w = m_texture->size().x();
    w -= tileSize;
    m_tileWidth = ( w / (margin + tileSize) ) + 1;

    m_gidMap = std::move(gidMap);
}

const RectI TiledSpritesheet::getRegion(int id) const
{
    return RectI{
        { (id % m_tileWidth) * (m_tileSize + m_margin), (id / m_tileWidth) * (m_tileSize + m_margin) },
        { m_tileSize, m_tileSize }
    };
}

const Vector2i TiledSpritesheet::sheetPosFromGid(int gid) const
{
    int x = (gid % m_tileWidth) * (m_tileSize + m_margin);
    int y = (gid / m_tileWidth) * (m_tileSize + m_margin);

    return {x, y};
}

Sprite TiledSpritesheet::spriteFromGid(int gid)
{
    auto sheetPos = sheetPosFromGid( gid );
    return Sprite {
        m_texture,
        { sheetPos, { m_tileSize, m_tileSize } }
    };
}

Sprite TiledSpritesheet::spriteFromName(std::string const &name)
{
    return spriteFromGid( m_gidMap.at(name) );
}

FreeSpritesheet::FreeSpritesheet(TexturePtr texture, SpritesheetRectMap const &rectMap)
: Spritesheet(texture), m_rectMap(rectMap)
{

}

Sprite FreeSpritesheet::spriteFromName(std::string const &name)
{
    auto& rect = m_rectMap.at(name);
    return Sprite { m_texture, rect };
}
