#include <graphics/Text.h>
#include <graphics/Texture.h>
#include <resource/Font.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <regex>

FontCache::FontCache(std::shared_ptr<Font>& font, Colour colour)
    : m_font(font)
{
    generateAsciiCache(font, colour);
}

void FontCache::generateAsciiCache(std::shared_ptr<Font>& font, Colour colour)
{
    const int maxW = 256;

    const int TotalCount = AsciiEndIdx - AsciiStartIdx;
    m_locs.reserve(TotalCount);
    m_metrics.reserve(TotalCount);

    int currX = 0;
    int currY = 0;

    std::vector<std::shared_ptr<Surface>> surfaceArr;

    for (int i = AsciiStartIdx; i < AsciiEndIdx; i++)
    {
        auto surface = font->renderGlyph( i, colour );
        surfaceArr.push_back( surface );

        int offsetX = surface->raw()->w + 1;

        if (currX + offsetX >= maxW)
        {
            currX = 0;
            currY += font->getLineSkip();
        }

        m_locs.push_back( SDL_Rect{currX, currY, surface->raw()->w, surface->raw()->w} );
        m_metrics.push_back( font->glyphMetric(i) );
        currX += offsetX;
    }

    currY += font->getLineSkip();
    Vector2i totalSize = { maxW, currY };

    auto cacheSurface = Surface::createSurface( totalSize );
    for (int i = 0; i < TotalCount; i++)
    {
        SDL_BlitSurface( surfaceArr[i]->raw(), nullptr, cacheSurface->raw(), &m_locs[i] );
    }

    m_cache = cacheSurface;
}

std::shared_ptr<Surface> FontCache::renderText(const std::string &text, int maxWidth )
{
    std::vector<GlyphPosition> glyphs;
    glyphs.reserve( text.size() );

    if (maxWidth <= 0)
    {
        maxWidth = std::numeric_limits<int>::max();
    }

    int currX = 0;
    int currY = 0;
    std::uint16_t previous = 0;

    for (int i = 0; i < text.size(); i++)
    {
        std::uint16_t idx = static_cast<std::uint16_t>( text.at(i) ) - AsciiStartIdx;
        int advance = m_metrics[idx].advance;

        glyphs.push_back( GlyphPosition{idx, currX, currY} );


        currX += advance;

        if (previous)
        {
            currX -= m_font->getKerning( previous, idx );
        }

        previous = idx;
    }

    auto outSurface = Surface::createSurface({currX, m_font->getLineSkip()});

    for ( int i = 0; i < glyphs.size(); i++)
    {
        GlyphPosition glpos = glyphs[i];
        SDL_Rect* glsize = &m_locs[glpos.idx];

        SDL_Rect outTransform = { glyphs[i].x, glyphs[i].y, glsize->w, glsize->h };
        SDL_BlitSurface( m_cache->raw(), glsize, outSurface->raw(), &outTransform );
    }

    return outSurface;
}

void FontCache::renderText(RenderInterface &ri, const std::string &text, int maxWidth)
{
    AssertNotImplemented();
}

//FontCache &FontManager::getFont(std::string fontName, int size, Colour colour)
//{
//    FontKey key = { fontName, size };
//    auto it = m_caches.find(key);
//    if ( it != m_caches.end() )
//    {
//        return it->second;
//    }
//    else
//    {
//        return generateFontAtlas(fontName, size, colour);
//    }
//}

//FontCache &FontManager::generateFontAtlas( std::string const& fontName, int size, Colour colour )
//{
//    auto font = ResourceManager::get().getFont( fontName, size );
//
//    FontCache fc {*font, colour};
//    FontKey key = { fontName, size };
//
//    auto [it, success] = m_caches.emplace(key, fc);
//    return it->second;
//}


void LiteMarkdownParser::parseMarkdown( std::string const& text )
{
    static const std::regex colourOpenRegex { R"(<colour:(\S+)>)" };
    static const std::regex colourCloseRegex { R"(</colour>)" };

    AssertNotImplemented();
}
