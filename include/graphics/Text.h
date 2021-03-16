#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <resource/Sprite.h>


struct FtCharData
{
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
    float uvX = 0.0f;
    float uvY = 0.0f;
};


class FtFontFace
{
public:
    FtFontFace( FT_Face const& face );
    ~FtFontFace();
    
    CompositeSprite renderString( std::string const& str, int fontSize );
    
    std::shared_ptr<Texture> texture();
    
private:

    void generateFontData( int size );
    
    
    static const int GLYPH_RANGE_MAX = 128;
    static const int TEXTURE_SIZE_PX = 256;
    static const int TEXTURE_PER_FONT_SIZE = 256;
    static const int SPACING_WIDTH = 2;
    static const int SPACING_HEIGHT = 2;

    Vector2i m_currentTexOffset;
    FT_Face m_face;
    std::shared_ptr<Texture> m_texture;
    std::unordered_map<int,std::vector<FtCharData>> m_charData;
};



class FtFontManager
{
public:
    FtFontManager();
    ~FtFontManager();
    
    std::shared_ptr<FtFontFace> getFont( std::string const& name, int size );
    std::shared_ptr<FtFontFace> loadFontFace( std::string const& name, int height );

private:

    std::map<std::pair<std::string, int>, std::shared_ptr<FtFontFace>> m_fonts;
    FT_Library m_ft;
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