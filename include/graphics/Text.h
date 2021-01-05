#pragma once

#include <unordered_map>

#include <utils/Colour.h>
#include <utils/Containers.h>
#include <resource/Font.h>

struct SDL_Texture;
struct SDL_Surface;
class Texture;
class Font;

class FontCache
{
public:

    FontCache( Font const& font, Colour colour );
    ~FontCache() = default;

    FontCache( const FontCache& ) = delete;
    FontCache& operator=( const FontCache& ) = delete;

private:

    void generateAsciiCache( Font const& font, Colour colour );

    std::vector<RectI> m_asciiLocs;
    std::vector<GlyphMetric> m_metrics;
    std::unique_ptr<Texture> m_cache;
};

using FontKey = std::pair<std::string, int>;


class FontManager
{
public:



private:
    std::unordered_map<FontKey, FontCache> m_caches;

};