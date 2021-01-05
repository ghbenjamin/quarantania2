#include <resource/Font.h>

#include <SDL2/SDL_ttf.h>

#include <utils/Assert.h>
#include <graphics/Texture.h>
#include <resource/ResourceManager.h>

Font::Font( TTF_Font* ptr, int size )
: m_font(ptr), m_size(size) {}

Font::~Font()
{
    if (m_font != nullptr)
    {
        TTF_CloseFont(m_font);
    }
}

FontPtr Font::loadFont(std::string const& path, int size)
{
    TTF_Font* font = TTF_OpenFont( path.c_str(), size );
    AssertMsg( font, TTF_GetError() );
    return std::make_shared<Font>(font, size);
}

std::shared_ptr<Texture> Font::renderText(std::string const &text, Colour colour, int wrapWidth)
{
    SDL_Surface* surface;

    if (wrapWidth > 0)
    {
        surface = TTF_RenderText_Blended_Wrapped( m_font, text.c_str(), colour.raw(), wrapWidth );
    }
    else
    {
        surface = TTF_RenderText_Blended( m_font, text.c_str(), colour.raw() );
    }

    Assert(surface != nullptr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        ResourceManager::get().getWindow()->renderer()->raw(), surface );

    Assert(texture != nullptr);

    SDL_FreeSurface(surface);

    return std::make_shared<Texture>(texture);
}

int Font::size() const
{
    return m_size;
}

TTF_Font *Font::raw() const
{
    return m_font;
}

std::shared_ptr<Surface> Font::renderGlyph(uint16_t glyph, Colour colour) const
{
    SDL_Surface* surface = TTF_RenderGlyph_Blended( m_font, glyph, colour.raw() );
    return std::make_shared<Surface>(surface);
}

GlyphMetric Font::glyphMetric( std::uint16_t glyph) const
{
    GlyphMetric metric;
    TTF_GlyphMetrics( m_font, glyph, &metric.minX, &metric.maxX, &metric.minY, &metric.maxY, &metric.advance );
    return metric;
}

int Font::getLineSkip() const
{
    return TTF_FontLineSkip( m_font );
}

int Font::getKerning(std::uint16_t previous, std::uint16_t next)
{
    return TTF_GetFontKerningSizeGlyphs( m_font, previous, next );
}
