#include <resource/Font.h>

#include <SDL2/SDL_ttf.h>

#include <utils/Assert.h>
#include <graphics/Texture.h>
#include <resource/ResourceManager.h>

FontData::FontData(TTF_Font* ptr, int size )
: m_font(ptr), m_size(size) {}

FontData::~FontData()
{
    if (m_font != nullptr)
    {
        TTF_CloseFont(m_font);
    }
}

FontDataPtr FontData::loadFont(std::string const& path, int size)
{
    TTF_Font* font = TTF_OpenFont( path.c_str(), size );
    AssertMsg( font, TTF_GetError() );
    return std::make_shared<FontData>(font, size);
}

std::shared_ptr<Texture> FontData::renderText(std::string const &text, Colour colour, int wrapWidth)
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
    auto managed = std::make_shared<Surface>(surface, PixelFormat::ARGB);
    
    return managed->toTexture();
}

int FontData::size() const
{
    return m_size;
}

TTF_Font *FontData::raw() const
{
    return m_font;
}

std::shared_ptr<Surface> FontData::renderGlyph(uint16_t glyph, Colour colour) const
{
    SDL_Surface* surface = TTF_RenderGlyph_Blended( m_font, glyph, colour.raw() );
    return std::make_shared<Surface>(surface, PixelFormat::ARGB);
}

GlyphMetric FontData::glyphMetric(std::uint16_t glyph) const
{
    GlyphMetric metric;
    TTF_GlyphMetrics( m_font, glyph, &metric.minX, &metric.maxX, &metric.minY, &metric.maxY, &metric.advance );
    return metric;
}

int FontData::getLineSkip() const
{
    return TTF_FontLineSkip( m_font );
}

int FontData::getKerning(std::uint16_t previous, std::uint16_t next)
{
    return TTF_GetFontKerningSizeGlyphs( m_font, previous, next );
}

//DirectRenderFont::DirectRenderFont(const FontPtr &fontData) : AbstractFont(fontData)
//{}

AbstractFont::AbstractFont(const FontDataPtr &fontData)
: m_fontData(fontData) {}

std::shared_ptr<Texture> DirectRenderFont::renderText(const std::string &text, Colour colour, int wrapWidth) const
{
    SDL_Surface* surface;

    if (wrapWidth > 0)
    {
        surface = TTF_RenderText_Blended_Wrapped( m_fontData->raw(), text.c_str(), colour.raw(), wrapWidth );
    }
    else
    {
        surface = TTF_RenderText_Blended( m_fontData->raw(), text.c_str(), colour.raw() );
    }

    Assert(surface != nullptr);
    auto managed = std::make_shared<Surface>(surface, PixelFormat::ARGB);
    
    return managed->toTexture();
}

std::shared_ptr<Surface> DirectRenderFont::renderGlyph(uint16_t glyph, Colour colour) const
{
    SDL_Surface* surface = TTF_RenderGlyph_Blended( m_fontData->raw(), glyph, colour.raw() );
    return std::make_shared<Surface>(surface, PixelFormat::ARGB);
}
