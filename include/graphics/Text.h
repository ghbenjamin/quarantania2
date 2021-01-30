#pragma once

#include <unordered_map>
#include <map>
#include <optional>

#include <utils/Colour.h>
#include <utils/Containers.h>
#include <resource/Font.h>

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Rect;
class Texture;
class FontData;
class RenderInterface;


class FontCache
{
public:

    struct GlyphPosition
    {
        int idx;
        int x;
        int y;
    };


    FontCache(std::shared_ptr<FontData> const& font, Colour colour );
    ~FontCache() = default;

    std::shared_ptr<Surface> renderText( std::string const& text, int maxWidth = -1 );

private:
    void generateAsciiCache(std::shared_ptr<FontData> const& font, Colour colour );
    static constexpr bool idxIsAlphanum( int idx );

    static constexpr int AsciiStartIdx = 32;
    static constexpr int AsciiEndIdx = 128;

    std::shared_ptr<FontData> m_font;
    std::vector<SDL_Rect> m_locs;
    std::vector<GlyphMetric> m_metrics;
    std::shared_ptr<Surface> m_cache;
};


class FontManager
{
public:

    FontManager() = default;
    ~FontManager() = default;

    // Return a reference to the specified font atlas, or create it if it does not exist.
    FontCache& getFont( std::string fontName, int size, Colour colour = Colour::Black );

    // Generate an atlas for the specified font, size, and colour
    FontCache& generateFontAtlas( std::string const& fontName, int size, Colour colour = Colour::Black );

private:
    std::map<std::string, std::map<std::pair<int, Colour>, FontCache>> m_fontCaches;
};




struct MarkdownTokenSegment
{
    Colour colour;
    std::string text;
};

using MarkdownTokenStream = std::vector<MarkdownTokenSegment>;


class LiteMarkdownParser
{

    struct OpenTagPosition
    {
        std::size_t start;
        std::size_t end;
        std::string value;
    };

    struct EndTagPosition
    {
        std::size_t start;
        std::size_t end;
    };

public:
    MarkdownTokenStream parseMarkdown( std::string const& text );
};