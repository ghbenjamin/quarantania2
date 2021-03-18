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
    float width;
    float height;
    float bearingX;
    float bearingY;
    int advance;
    RectF uv;
};


struct FtCharPlacement
{
    float x;
    float y;
    std::size_t idx;
};


class TextRenderObj
{
public:
    TextRenderObj();
    TextRenderObj( TexturePtr texture );
    ~TextRenderObj() = default;

    void addQuad( RectF screenOffsets, RectF uvBounds );
    void setPosition( Vector2f pos );
    void setSize( Vector2i size );
    void setColour( Colour colour );

    RenderObject& renderObject( Vector2i pos );
    Vector2i getSize() const;

private:
    std::size_t m_charCount;
    TexturePtr m_texture;
    Vector2i m_size;
    RenderObject m_renderObj;
    std::vector<RectF> m_charBounds;
};


class FtFontFace
{
public:
    FtFontFace( FT_Face const& face );
    ~FtFontFace();
    
    TextRenderObj renderString( std::string const& str, int fontSize );
    TextRenderObj renderString( std::string const& str, int fontSize, int maxWidth );

    Sprite renderGlyph( char c, int fontSize );

private:

    // Generate the texture atlas for our font with the given size and store it in our font texture
    void generateFontData( int size );

    static const int GLYPH_RANGE_MAX = 128;
    static const int TEXTURE_WIDTH_PX = 256;
    static const int TEXTURE_HEIGHT_PX = 512;
    static const int TEXTURE_PER_FONT_SIZE = 256;
    static const int SPACING_WIDTH = 2;
    static const int SPACING_HEIGHT = 2;

    Vector2f m_currentTexOffset;
    FT_Face m_face;
    std::shared_ptr<Texture> m_texture;
    std::unordered_map<int,std::vector<FtCharData>> m_charData;
};



class FtFontManager
{
public:
    FtFontManager();
    ~FtFontManager();
    
    std::shared_ptr<FtFontFace> getFont( std::string const& name );
    std::shared_ptr<FtFontFace> loadFontFace( std::string const& name );

private:
    std::map<std::string, std::shared_ptr<FtFontFace>> m_fonts;
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