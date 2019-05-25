#pragma once

#include <string>
#include <unordered_map>

#include <resource/Texture.h>
#include <resource/Sprite.h>

using SpritesheetGidMap = std::unordered_map<std::string, int>;
using SpritesheetKey = std::pair<std::string, std::string>;

class Spritesheet
{
public:

    Spritesheet( TexturePtr texture, SpritesheetGidMap& gidMap, int margin, int tileSize );
    ~Spritesheet() = default;

    Spritesheet( const Spritesheet& ) = delete;
    Spritesheet& operator=( const Spritesheet& ) = delete;

    const RectI getRegion( int id ) const;
    const Vector2i sheetPosFromGid( int gid ) const;

    Sprite spriteFromGid( int gid );
    Sprite spriteFromName( std::string const& name );

private:

    int m_margin;
    int m_tileSize;
    int m_tileWidth;

    TexturePtr m_texture;
    SpritesheetGidMap m_gidMap;

};

using SpritesheetPtr = std::shared_ptr<Spritesheet>;
