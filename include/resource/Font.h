#pragma once

#include <memory>
#include <string>
#include <utils/Colour.h>
#include <SDL_ttf.h>


// Forward Definitions
class FontData;
class Texture;
class Surface;

// Typedefs
using FontDataPtr = std::shared_ptr<FontData>;


struct GlyphMetric
{
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;
    int advance = 0;
};



class AbstractFont
{
public:

    AbstractFont(FontDataPtr const& fontData);
    virtual ~AbstractFont() = default;


    virtual std::shared_ptr<Texture>
        renderText( std::string const& text, Colour colour=Colour::Black, int wrapWidth=-1 ) const = 0;
    virtual std::shared_ptr<Surface>
        renderGlyph( uint16_t glyph, Colour colour ) const = 0;

protected:
    FontDataPtr m_fontData;
};


class DirectRenderFont : public AbstractFont
{
public:

    using AbstractFont::AbstractFont;

//    DirectRenderFont(FontPtr const& fontData);
//    ~DirectRenderFont() override = default;

    std::shared_ptr<Texture> renderText( std::string const& text, Colour colour=Colour::Black, int wrapWidth=-1 ) const override;
    std::shared_ptr<Surface> renderGlyph( uint16_t glyph, Colour colour ) const override;
};


class FontData
{
public:

    static FontDataPtr loadFont(std::string const& path, int size );

    FontData(TTF_Font* ptr, int size );
    ~FontData();

    FontData(const FontData& ) = delete;
    FontData& operator=(const FontData& ) = delete;

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