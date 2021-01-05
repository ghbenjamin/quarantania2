#include <graphics/Text.h>
#include <graphics/Texture.h>
#include <resource/Font.h>

FontCache::FontCache(const Font &font, Colour colour)
{
    generateAsciiCache(font, colour);
}

void FontCache::generateAsciiCache(const Font &font, Colour colour)
{
    const int lineSkip = font.getLineSkip();
    const int maxW = 256;
    const int asciiOffset = 32;
    const int asciiMax = 128;

    const int TotalCount = asciiMax - asciiOffset;
    m_asciiLocs.reserve(TotalCount);
    m_metrics.reserve(TotalCount);

    int currX = 0;
    int currY = 0;

    std::vector<std::shared_ptr<Surface>> surfaceArr;

    for (int i = asciiOffset; i < asciiMax; i++)
    {
        auto surface = font.renderGlyph( i, colour );
        surfaceArr.push_back( surface );

        int offsetX = surface->raw()->w + 1;

        if (currX + offsetX >= maxW)
        {
            currX = 0;
            currY += lineSkip;
        }

        m_asciiLocs.emplace_back( currX, currY, surface->raw()->w, surface->raw()->w );
        m_metrics.push_back( font.glyphMetric(i) );
        currX += offsetX;
    }

    currY += lineSkip;
    Vector2i totalSize = { maxW, currY };

    auto cacheSurface = Surface::createSurface( totalSize );
    for (int i = 0; i < TotalCount; i++)
    {
        SDL_Rect rect = m_asciiLocs[i].toSDL();
        SDL_BlitSurface( surfaceArr[i]->raw(), nullptr, cacheSurface->raw(), &rect );
    }

    m_cache = std::make_unique<Texture>(cacheSurface);
}
