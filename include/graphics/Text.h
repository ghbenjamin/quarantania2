#pragma once

#include <unordered_map>

#include <utils/Colour.h>
#include <utils/Containers.h>
#include <resource/Font.h>

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Rect;
class Texture;
class Font;
class RenderInterface;


struct GlyphPosition
{
    int idx;
    int x;
    int y;
};



class FontCache
{
public:

    FontCache( std::shared_ptr<Font>& font, Colour colour );
    ~FontCache() = default;

    std::shared_ptr<Surface> renderText( std::string const& text, int maxWidth = -1 );

    void renderText( RenderInterface& oi, std::string const& text, int maxWidth );

private:
    void generateAsciiCache( std::shared_ptr<Font>& font, Colour colour );

    static constexpr int AsciiStartIdx = 32;
    static constexpr int AsciiEndIdx = 128;

    std::shared_ptr<Font> m_font;
    std::vector<SDL_Rect> m_locs;
    std::vector<GlyphMetric> m_metrics;
    std::shared_ptr<Surface> m_cache;
};

using FontKey = std::pair<std::string, int>;


class FontManager
{
public:

    FontManager() = default;
    ~FontManager() = default;

    // Return a reference to the specified font atlas, or create it if it does not exist.
//    FontCache& getFont( std::string fontName, int size, Colour colour = Colour::Black );

    // Generate an atlas for the specified font, size, and colour
//    FontCache& generateFontAtlas( std::string const& fontName, int size, Colour colour = Colour::Black );

private:
    std::unordered_map<FontKey, FontCache> m_caches;

};



struct MarkdownToken
{
    std::string text;
    bool isBold;
    bool isItalic;
    Colour colour;
};


class LiteMarkdownParser
{
public:
    void parseMarkdown( std::string const& text );
};