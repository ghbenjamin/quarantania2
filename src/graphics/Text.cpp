#include <graphics/Text.h>

#include <regex>

#include <graphics/Texture.h>
#include <resource/Font.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>

FontCache::FontCache(std::shared_ptr<FontData> const& font, Colour colour)
    : m_font(font)
{
    generateAsciiCache(font, colour);
}

void FontCache::generateAsciiCache(std::shared_ptr<FontData> const& font, Colour colour)
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

    auto cacheSurface = std::make_shared<Surface>( totalSize );
    for (int i = 0; i < TotalCount; i++)
    {
        SDL_BlitSurface( surfaceArr[i]->raw(), nullptr, cacheSurface->raw(), &m_locs[i] );
    }

    m_cache = cacheSurface;
}

std::shared_ptr<Surface> FontCache::renderText(const std::string &text, int maxWidth )
{
    std::vector<GlyphPosition> glyphs;
    std::vector<GlyphPosition> currentWord;

    glyphs.reserve( text.size() );

    // If w < 0, assume width is unbounded
    if (maxWidth <= 0)
    {
        maxWidth = std::numeric_limits<int>::max();
    }

    const int LineSpacing = m_font->getLineSkip();
    int currX = 0;
    int currY = 0;
    std::uint16_t previous = 0;
    int maxX = 0;

    // For each character in the input string:
    //   - Work out the size of the glyph
    //   - Work out which word it belongs to
    //   - If this glyph is the end of a word, work out whether or not the word should go onto the current line or
    //     the next line, depending on our requested text wrap width

    for (int i = 0; i < (int) text.size(); i++)
    {
        // Convert from char to unicode codepoint, offset to start at the alphanumeric characters
        std::uint16_t idx_noffset = static_cast<std::uint16_t>( text.at(i) );
        std::uint16_t idx = idx_noffset - AsciiStartIdx;

        currentWord.push_back( GlyphPosition{idx, currX, currY} );


        // Move the caret forward the width of the current glyph
        int advance = m_metrics[idx].advance;
        currX += advance;

        // If we have a previous glyph, apply kerning offsets
        if (previous)
        {
            currX -= m_font->getKerning( previous, idx );
        }

        previous = idx;

        // Non-alphanumeric means this could be the end of a word
        if ( !idxIsAlphanum( idx_noffset ) )
        {
            // The left and right edges of the current word (in pixels)
            int frontEdge = currentWord.front().x;
            int backEdge = currentWord.back().x + m_locs[idx].w;

            if ( backEdge > maxWidth )
            {
                // The current word extends beyond the max width - move the current word down to the next line.
                for (auto& c : currentWord)
                {
                    c.x -= frontEdge;
                    c.y += LineSpacing;
                }

                // Reset our current X and Y positions since we've moved the caret
                maxX = std::max( maxX, frontEdge );
                currX = backEdge - frontEdge;
                currY += LineSpacing;
            }

            // Move the completed word onto the glyph vector and clear the current word
            glyphs.insert( glyphs.end(), currentWord.begin(), currentWord.end() );
            currentWord.clear();
        }
    }

    // In case this was a single line, maxX should be at least the size of currX
    maxX = std::max( maxX, currX );

    auto outSurface = std::make_shared<Surface>( Vector2i{maxX, currY + LineSpacing} );

    // For each character, blit the correct glyph onto the target surface
    for ( int i = 0; i < (int) glyphs.size(); i++)
    {
        GlyphPosition glpos = glyphs[i];
        SDL_Rect* glsize = &m_locs[glpos.idx];

        SDL_Rect outTransform = { glyphs[i].x, glyphs[i].y, glsize->w, glsize->h };
        SDL_BlitSurface( m_cache->raw(), glsize, outSurface->raw(), &outTransform );
    }

    return outSurface;
}

constexpr bool FontCache::idxIsAlphanum(int idx)
{
    // Quick and dirty - return whether this index falls within 0-9, a-z, A-Z

    return ( idx >= 48 && idx <= 57 ) ||
           ( idx >= 65 && idx <= 90 ) ||
           ( idx >= 97 && idx <= 122);
}

FontCache &FontManager::getFont(std::string fontName, int size, Colour colour)
{
    auto family = m_fontCaches.find( fontName );

    if ( family != m_fontCaches.end() )
    {
        std::pair<int, Colour> key = std::make_pair(size, colour);

        auto font_it = family->second.find( key );
        if (font_it != family->second.end())
        {
            return font_it->second;
        }
    }

    return generateFontAtlas( fontName, size, colour );
}

FontCache &FontManager::generateFontAtlas( std::string const& fontName, int size, Colour colour )
{
    auto font = ResourceManager::get().getFont( fontName, size );

    FontCache fc { font, colour };
    std::pair<int, Colour> key = std::make_pair(size, colour);

    auto it = m_fontCaches.find(fontName);

    if (it != m_fontCaches.end() )
    {
        it->second.emplace( key, fc );
    }
    else
    {
        m_fontCaches[fontName] = {};
        m_fontCaches[fontName].emplace( key, fc );
    }

    return m_fontCaches[fontName].at( key );
}

MarkdownTokenStream LiteMarkdownParser::parseMarkdown( std::string const& text )
{
    // Example markdown:
    // "The <c:#ff0000>quick brown</c> fox jumped <c:#00ff00>over the lazy</c> dog."

    Colour const& DefaultColour = Colour::Black;

    std::regex tagOpen { R"(<c:(\S+)>)" };
    std::regex tagClose { R"(</c>)" };

    std::sregex_iterator openIt {text.begin(), text.end(), tagOpen };
    std::sregex_iterator closeIt {text.begin(), text.end(), tagClose };
    std::sregex_iterator end;

    std::vector<OpenTagPosition> openTags;
    std::vector<EndTagPosition> closeTags;


    for ( ; openIt != end; openIt++)
    {
        OpenTagPosition tp;
        tp.start = openIt->position();
        tp.end = tp.start + openIt->length();
        tp.value = openIt->str(1);

        openTags.push_back(tp);
    }

    for ( ; closeIt != end; closeIt++)
    {
        EndTagPosition tp;
        tp.start = closeIt->position();
        tp.end = tp.start + closeIt->length();

        closeTags.push_back(tp);
    }

    AssertMsg( openTags.size() == closeTags.size(), "Open and close tag mismatch" );

    // If we have no tags, return a simple black single element stream
    if ( openTags.size() == 0 )
    {
        return MarkdownTokenStream{ MarkdownTokenSegment{ Colour::Black, text } };
    }

    // Otherwise:

    MarkdownTokenStream out;

    std::size_t lastIdx = 0;

    for ( int i = 0; i < openTags.size(); i++ )
    {
        out.push_back( MarkdownTokenSegment {
            DefaultColour,
            text.substr( lastIdx, openTags[i].start - lastIdx )
        });

        out.push_back( MarkdownTokenSegment {
            Colour{ openTags[i].value },
            text.substr( openTags[i].end, closeTags[i].start - openTags[i].end )
        });

        lastIdx = closeTags[i].end;
    }

    out.push_back( MarkdownTokenSegment {
        DefaultColour,
        text.substr( closeTags.back().end, std::string::npos )
    });

    return out;
}
