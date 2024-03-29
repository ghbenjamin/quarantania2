#pragma once

#include <string>
#include <unordered_map>

#include <graphics/Texture.h>
#include <resource/Sprite.h>

using SpritesheetGidMap = std::unordered_map<std::string, int>;
using SpritesheetRectMap = std::unordered_map<std::string, RectI>;

struct SpritesheetKey
{
    SpritesheetKey();

    // Sheet name and sprite name
    SpritesheetKey(std::string const& sheet, std::string const& name);

    // Sheet name and gid
    SpritesheetKey(std::string const& sheet, int gid);

    // Sheet name and sprite name as a single string, separated by a '/'
    SpritesheetKey(std::string const& key);
    SpritesheetKey(const char* key);

    ~SpritesheetKey() = default;

    // String representation (mostly for debugging)
    std::string toString() const;

    std::string sheetName;
    std::string spriteName;
    int gid;
};


class Spritesheet
{
public:
    Spritesheet( TexturePtr texture );
    virtual ~Spritesheet() = default;

    Spritesheet( const Spritesheet& ) = delete;
    Spritesheet& operator=( const Spritesheet& ) = delete;

    virtual Sprite getSprite( SpritesheetKey const& key ) = 0;
    TexturePtr const& getTexture();

protected:
    TexturePtr m_texture;
};


class TiledSpritesheet : public Spritesheet
{
public:
    TiledSpritesheet( TexturePtr texture, SpritesheetGidMap& gidMap, int spriteMargin, int globalMargin );
    ~TiledSpritesheet() override = default;

    Sprite spriteFromGid( int gid );
    const Vector2i sheetPosFromGid( int gid ) const;
    virtual Sprite getSprite( SpritesheetKey const& key ) override;

private:

    int m_margin;
    int m_tileWidth;
    int m_globalMargin;
    Vector2i m_globalOffset;
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
