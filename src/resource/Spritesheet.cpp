#include <resource/Spritesheet.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <utils/GlobalConfig.h>

SpritesheetKey::SpritesheetKey()
{
    this->gid = -1;
    this->sheetName = "";
    this->spriteName = "";
}

SpritesheetKey::SpritesheetKey(const std::string &sheet, const std::string &name)
{
    this->spriteName = name;
    this->sheetName = sheet;
    this->gid = -1;
}

SpritesheetKey::SpritesheetKey(std::string const& sheet, int gid)
{
    this->spriteName = "";
    this->gid = gid;
    this->sheetName = sheet;
}

SpritesheetKey::SpritesheetKey(const std::string &key)
{
    auto idx = key.find('/');
    this->sheetName = key.substr(0, idx);

    idx++;
    if ( std::isdigit(key.at(idx)) )
    {
        this->gid = std::stoi(key.substr(idx, key.size()));
        this->spriteName = "";
    }
    else
    {
        this->spriteName = key.substr(idx, key.size());
        this->gid = -1;
    }
}

SpritesheetKey::SpritesheetKey(const char *key)
 : SpritesheetKey( std::string(key) )
 {}


Spritesheet::Spritesheet(TexturePtr texture)
: m_texture(texture)
{

}

TiledSpritesheet::TiledSpritesheet(TexturePtr texture, SpritesheetGidMap& gidMap, int margin)
: Spritesheet(texture), m_margin(margin)
{
    auto w = m_texture->size().x();
    w -= GlobalConfig::TileSizePx;
    m_tileWidth = ( w / (margin + GlobalConfig::TileSizePx) ) + 1;

    m_gidMap = std::move(gidMap);
}

const RectI TiledSpritesheet::getRegion(int id) const
{
    return RectI{
        { (id % m_tileWidth) * (GlobalConfig::TileSizePx + m_margin),
          (id / m_tileWidth) * (GlobalConfig::TileSizePx + m_margin) },
        { GlobalConfig::TileSizePx,
          GlobalConfig::TileSizePx }
    };
}

const Vector2i TiledSpritesheet::sheetPosFromGid(int gid) const
{
    int x = (gid % m_tileWidth) * (GlobalConfig::TileSizePx + m_margin);
    int y = (gid / m_tileWidth) * (GlobalConfig::TileSizePx + m_margin);

    return {x, y};
}

Sprite TiledSpritesheet::spriteFromGid(int gid)
{
    auto sheetPos = sheetPosFromGid( gid );
    return Sprite {
        m_texture,
        { sheetPos, { GlobalConfig::TileSizePx, GlobalConfig::TileSizePx } }
    };
}

Sprite TiledSpritesheet::getSprite( SpritesheetKey const& key )
{
    if (key.gid >=0)
    {
        return spriteFromGid( key.gid );
    }
    else
    {
        return spriteFromGid( m_gidMap.at(key.spriteName) );
    }
}

FreeSpritesheet::FreeSpritesheet(TexturePtr texture, SpritesheetRectMap const &rectMap)
: Spritesheet(texture), m_rectMap(rectMap)
{

}

Sprite FreeSpritesheet::getSprite( SpritesheetKey const& key )
{
    auto& rect = m_rectMap.at(key.spriteName);
    return Sprite { m_texture, rect };
}

