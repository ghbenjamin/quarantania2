#include <graphics/Text.h>

#include <iostream>
#include <regex>
#include <locale>

#include <utils/Assert.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>


// Font Manager


FtFontManager::FtFontManager()
{
    if ( FT_Init_FreeType(&m_ft) )
    {
        AssertAlways();
    }
}

FtFontManager::~FtFontManager()
{
    m_fonts.clear();
    FT_Done_FreeType(m_ft);
}

std::shared_ptr<FtFontFace> FtFontManager::loadFontFace( std::string const &name )
{
    std::string path = "../resource/font/" + name + ".ttf";
    
    FT_Face face;
    if (FT_New_Face(m_ft, path.c_str(), 0, &face))
    {
        AssertAlwaysMsg( "Unknown font: " + path );
    }
    
    auto font = std::make_shared<FtFontFace>(face);
    m_fonts.emplace( name, font );
    
    return font;
}

std::shared_ptr<FtFontFace> FtFontManager::getFont( std::string const &name )
{
    auto it = m_fonts.find( name );
    
    if (it != m_fonts.end())
    {
        return it->second;
    }
    else
    {
        return loadFontFace(name);
    }
}



// Font Face


FtFontFace::FtFontFace( FT_Face const& face )
    : m_face(face), m_currentTexOffset(0, 0)
{
    m_texture = std::make_shared<Texture>( Vector2i{ TEXTURE_WIDTH_PX, TEXTURE_HEIGHT_PX } );
    generateFontData( 28 );
    generateFontData( 18 );
    generateFontData( 16 );
    generateFontData( 14 );
    generateFontData( 12 );
    generateFontData( 10 );
}

FtFontFace::~FtFontFace()
{
    FT_Done_Face(m_face);
}

TextRenderObj FtFontFace::renderString( std::string const &str, int fontSize, int maxWidth )
{
    // Grab the character data for the requested font size
    auto it = m_charData.find(fontSize);
    AssertMsg( it != m_charData.end(), fmt::format("Missing font size: {}", fontSize) );
    auto& charVector = it->second;

    // Max width <=0 => unbounded width, never wrap
    if (maxWidth <= 0)
    {
        maxWidth = std::numeric_limits<int>::max();
    }

    std::vector<FtCharPlacement> allGlyphs;
    std::vector<FtCharPlacement> currentWordGlyphs;

    
    float currX = 0;
    float currY = 0;
    float nextX = 0;
    float nextY = 0;
    float lineSpacing = (float) fontSize; // This is wrong <---
//    float lineSpacing = (float) (m_face->height / 64);

    float maxY = -1000;
    float minY = 1000;
    float maxH = 0;
    float maxW = 0;

    int lineCount = 1;
    
    for (int i = 0; i < (int) str.size(); i++)
    {
        std::uint16_t char_idx = static_cast<std::uint16_t>( str.at(i) );
        auto& char_data = charVector[char_idx];

        auto placement = FtCharPlacement{ currX + char_data.bearingX, currY - char_data.bearingY, char_idx };
        currentWordGlyphs.push_back(placement);

        currX += char_data.advance;
        maxH = std::max(maxH, char_data.height);
        maxY = std::max(placement.y + char_data.height, maxY);
        minY = std::min(placement.y, minY);

        if ( !std::isalnum( str.at(i)) )
        {
            // If the current glyph is not alphanumeric, it might be the end of a word. Consider this glyph a possible
            // break point and work out whether or not we need to wrap onto the next line

            float leftWordEdge = currentWordGlyphs.front().x;
            float currGlyphWidth = (float) charVector[currentWordGlyphs.back().idx].advance;
            float rightWordEdge = currentWordGlyphs.back().x + currGlyphWidth;

            if (rightWordEdge > maxWidth)
            {
                // The right-most edge of the current word extends beyond the wrap width - the current word needs
                // to be moved to the beginning of the next line.

                for ( auto& c : currentWordGlyphs )
                {
                    c.x -= leftWordEdge;
                    c.y += lineSpacing;
                }

                currX = rightWordEdge - leftWordEdge;
                currY += lineSpacing;
                maxW = std::max( maxW, leftWordEdge );
                lineCount++;
            }

            allGlyphs.insert( allGlyphs.end(), currentWordGlyphs.begin(), currentWordGlyphs.end() );
            currentWordGlyphs.clear();
        }
    }

    // NEEDS THOUGHT
    if (!currentWordGlyphs.empty())
    {
        allGlyphs.insert( allGlyphs.end(), currentWordGlyphs.begin(), currentWordGlyphs.end() );
    }
    
    auto textObj = TextRenderObj{ m_texture };
    
    for (auto& c : allGlyphs )
    {
        c.y += maxH;
    
        textObj.addQuad(
            RectF{c.x, c.y, charVector[c.idx].width, charVector[c.idx].height },
            charVector[c.idx].uv
        );
    }

    maxW = std::max( maxW, allGlyphs.back().x + charVector[allGlyphs.back().idx].width - allGlyphs.front().x );

    textObj.setSize({
        (int) maxW,
        (int)(lineCount * lineSpacing)
    });

    return textObj;
}

Sprite FtFontFace::renderGlyph( char c, int fontSize )
{
    auto it = m_charData.find(fontSize);
    AssertMsg( it != m_charData.end(), "Missing font size" );
    auto& charVector = it->second;
    
    auto data = charVector[static_cast<std::uint16_t>(c)];
    
    RectI bounds = RectF{ data.uv.x() * TEXTURE_WIDTH_PX, data.uv.y() * TEXTURE_HEIGHT_PX,
        data.uv.w() * TEXTURE_WIDTH_PX, data.uv.h() * TEXTURE_HEIGHT_PX }.convert<int>();
    
    return Sprite { m_texture, bounds };
}


void FtFontFace::generateFontData( int size )
{
    FT_Set_Pixel_Sizes(m_face, 0, size);

    std::vector<FtCharData> charVector;
    charVector.reserve(GLYPH_RANGE_MAX);
    
    float currX = m_currentTexOffset.x();
    float currY = m_currentTexOffset.y();

    float nextX = 0;
    float nextY = 0;

    float currMaxH = 0;

    // For each glyph in the range of ascii values 0 to 128
    for (int ch = 0; ch < GLYPH_RANGE_MAX; ch++)
    {
        if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
        {
            AssertAlwaysMsg( "Error loading char" );
        }
        
        FtCharData charData;
        
        charData.width = (float) m_face->glyph->bitmap.width;
        charData.height = (float) m_face->glyph->bitmap.rows;
        charData.bearingX = (float) m_face->glyph->bitmap_left;
        charData.bearingY = (float) m_face->glyph->bitmap_top;
        charData.advance = m_face->glyph->advance.x >> 6;
        
        currMaxH = std::max(charData.height, currMaxH);


        float offset = charData.width + SPACING_WIDTH;
        if (currX + offset >= TEXTURE_PER_FONT_SIZE + m_currentTexOffset.x())
        {
            // Not enough room for this glyph on the current line - move to the next one
            currY = currY + currMaxH + SPACING_HEIGHT;
            currX = m_currentTexOffset.x();

            nextY = currY;
            nextX = offset;

            currMaxH = charData.height;
        }
        else
        {
            // There's enough space for the next character, move the cursor along
            nextY = currY;
            nextX = currX + offset;
        }


        charData.uv.x( currX / (float) TEXTURE_WIDTH_PX );
        charData.uv.y( currY / (float) TEXTURE_HEIGHT_PX );
        charData.uv.w( charData.width / (float) TEXTURE_WIDTH_PX );
        charData.uv.h( charData.height / (float) TEXTURE_HEIGHT_PX );
        
        glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                (int) currX,
                (int) currY,
                (int) charData.width,
                (int) charData.height,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                m_face->glyph->bitmap.buffer
        );


        currX = nextX;
        currY = nextY;
    
        charVector.push_back( std::move(charData) );
    }
 
 
    m_currentTexOffset = {0, nextY + currMaxH * 2};
    m_charData.emplace(size, std::move(charVector) );
}

TextRenderObj FtFontFace::renderString(const std::string &str, int fontSize)
{
    return renderString(str, fontSize, -1);
}


RenderObject &TextRenderObj::renderObject(Vector2i pos)
{
    setPosition( pos.convert<float>() );
    return m_renderObj;
}

Vector2i TextRenderObj::getSize() const
{
    return m_size;
}

TextRenderObj::TextRenderObj( TexturePtr texture )
    : m_texture(texture),
      m_renderObj(texture->handle(), ShaderType::TextShader),
      m_charCount(0) {}

TextRenderObj::TextRenderObj()
    : m_texture(0), m_renderObj({}), m_charCount(0) {}

void TextRenderObj::addQuad( RectF screenOffsets, RectF uvBounds )
{
    if (m_charCount == 0)
    {
        m_renderObj.setScreenVerts(0, screenOffsets.x(), screenOffsets.y(), screenOffsets.w(), screenOffsets.h() );
        m_renderObj.setTextureVerts(0, uvBounds.x(), uvBounds.y(), uvBounds.w(), uvBounds.h() );
    }
    else
    {
        m_renderObj.addQuad( screenOffsets, uvBounds );
    }
    
    m_charCount++;
    m_charBounds.push_back( screenOffsets );
}

void TextRenderObj::setPosition( Vector2f pos )
{
    for (int i = 0; i < (int) m_charCount; i++ )
    {
        m_renderObj.setScreenVerts(i, pos.x() + m_charBounds[i].x(), pos.y() + m_charBounds[i].y(),
            m_charBounds[i].w(), m_charBounds[i].h());
    }
}


void TextRenderObj::setColour( Colour colour )
{
    auto [r, b, g, a] = colour.asOpenGL();
    
    for (int i = 0; i < (int) m_charCount; i++)
    {
        m_renderObj.setColourVerts( i, r, g, b, a );
    }
}

void TextRenderObj::setSize( Vector2i size )
{
    m_size = size;
}
