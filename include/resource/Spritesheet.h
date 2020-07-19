#pragma once

#include <string>
#include <unordered_map>

#include <resource/Texture.h>
#include <resource/Sprite.h>

using SpritesheetGidMap = std::unordered_map<std::string, int>;
using SpritesheetRectMap = std::unordered_map<std::string, RectI>;

struct SpritesheetKey
{
    SpritesheetKey();
    SpritesheetKey(std::string const& sheet, std::string const& name);
    SpritesheetKey(std::string const& sheet, int gid );
    ~SpritesheetKey() = default;

    std::string spriteName;
    int gid;
    std::string sheetName;
};


class Spritesheet
{
public:
    Spritesheet( TexturePtr texture );
    virtual ~Spritesheet() = default;

    Spritesheet( const Spritesheet& ) = delete;
    Spritesheet& operator=( const Spritesheet& ) = delete;

    virtual Sprite getSprite( SpritesheetKey const& key ) = 0;

protected:
    TexturePtr m_texture;
};


class TiledSpritesheet : public Spritesheet
{
public:
    TiledSpritesheet( TexturePtr texture, SpritesheetGidMap& gidMap, int margin );
    ~TiledSpritesheet() override = default;

    Sprite spriteFromGid( int gid );
    const Vector2i sheetPosFromGid( int gid ) const;
    const RectI getRegion( int id ) const;

    virtual Sprite getSprite( SpritesheetKey const& key ) override;

private:

    int m_margin;
    int m_tileWidth;
    SpritesheetGidMap m_gidMap;
};


class FreeSpritesheet : public Spritesheet
{
public:
    FreeSpritesheet( TexturePtr texture, SpritesheetRectMap const& rectMap );
    ~FreeSpritesheet() override = default;

    Sprite getSprite( SpritesheetKey const& key ) override;

private:
    SpritesheetRectMap m_rectMap;
};


using SpritesheetPtr = std::shared_ptr<Spritesheet>;
