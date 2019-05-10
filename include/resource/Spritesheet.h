#pragma once

#include <resource/Texture.h>

class Spritesheet
{
public:

    Spritesheet( TexturePtr texture, int margin, int tileSize );
    ~Spritesheet() = default;

    Spritesheet( const Spritesheet& ) = delete;
    Spritesheet& operator=( const Spritesheet& ) = delete;

    const RectI getRegion( int id ) const;

private:

    int m_margin;
    int m_tileSize;
    int m_tileWidth;

    TexturePtr m_texture;

};

using SpritesheetPtr = std::shared_ptr<Spritesheet>;