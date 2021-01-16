#pragma once

#include <memory>
#include <string>
#include <utils/Colour.h>
#include <SDL_ttf.h>


// Forward Definitions
class Font;
class Texture;
class Surface;

// Typedefs
using FontPtr = std::shared_ptr<Font>;


struct GlyphMetric
{
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    int advance = 0;
};


class Font
{
public:

    static FontPtr loadFont( std::string const& path, int size );

    Font( TTF_Font* ptr, int size );
    ~Font();

    Font( const Font& ) = delete;
    Font& operator=( const Font& ) = delete;

    int size() const;
    TTF_Font* raw() const;

    int getLineSkip() const;

    std::shared_ptr<Texture> renderText( std::string const& text, Colour colour=Colour::Black, int wrapWidth=-1 );
    std::shared_ptr<Surface> renderGlyph( uint16_t glyph, Colour colour ) const;

    GlyphMetric glyphMetric( std::uint16_t glyph ) const;
    int getKerning( std::uint16_t previous, std::uint16_t next );

private:
    int m_size;
    TTF_Font* m_font;
};