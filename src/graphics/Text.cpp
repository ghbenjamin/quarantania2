#include <graphics/Text.h>

#include <iostream>
#include <regex>

#include <utils/Assert.h>
#include <utils/Logging.h>


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

TextRenderObj FtFontFace::renderString( std::string const &str, int fontSize )
{
    auto it = m_charData.find(fontSize);
    AssertMsg( it != m_charData.end(), fmt::format("Missing font size: {}", fontSize) );
    auto& charVector = it->second;
    
    std::vector<FtCharPlacement> char_placements;
    
    float maxH = 0;
    float currX = 0;
    float currY = 0;
    float nextX = 0;
    float nextY = 0;
    
    float maxY = -1000;
    float minY = 1000;
    
    for (int i = 0; i < (int) str.size(); i++)
    {
        std::uint16_t char_idx = static_cast<std::uint16_t>( str.at(i) );
        auto& char_data = charVector[char_idx];

        auto placement = FtCharPlacement{ currX + char_data.bearingX, currY - char_data.bearingY, char_idx };
        char_placements.push_back(placement);
        
        currX += char_data.advance >> 6;
        maxH = std::max(maxH, char_data.height);
        
        maxY = std::max(placement.y + char_data.height, maxY);
        minY = std::min(placement.y, minY);
    }
    
    auto textObj = TextRenderObj{ m_texture };
    
    for (auto& c : char_placements )
    {
        c.y += maxH;
    
        textObj.addQuad(
            RectF{c.x, c.y, charVector[c.idx].width, charVector[c.idx].height },
            charVector[c.idx].uv
        );
    }

    textObj.setSize({
        (int)(char_placements.back().x + charVector[char_placements.back().idx].width - char_placements.front().x),
        (int)(maxY - minY)
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


std::shared_ptr<Texture> FtFontFace::texture()
{
    return m_texture;
}

void FtFontFace::generateFontData( int size )
{
    FT_Set_Pixel_Sizes(m_face, 0, size);

    std::vector<FtCharData> charVector;
    charVector.reserve(GLYPH_RANGE_MAX);
    
    int currX = m_currentTexOffset.x();
    int currY = m_currentTexOffset.y();
    int nextX = 0;
    int nextY = 0;
    float currMaxH = 0;
    
    for (int ch = 0; ch < GLYPH_RANGE_MAX; ch++)
    {
        if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
        {
            AssertAlways();
        }
        
        FtCharData charData;
        
        charData.width = (float) m_face->glyph->bitmap.width;
        charData.height = (float) m_face->glyph->bitmap.rows;
        charData.bearingX = (float) m_face->glyph->bitmap_left;
        charData.bearingY = (float) m_face->glyph->bitmap_top;
        charData.advance = (float) m_face->glyph->advance.x;
        
        currMaxH = std::max(charData.height, currMaxH);
        
        int offset = charData.width + SPACING_WIDTH;
        if (currX + offset > TEXTURE_PER_FONT_SIZE + m_currentTexOffset.x())
        {
            nextY = currY + currMaxH + SPACING_HEIGHT;
            nextX = m_currentTexOffset.x();
            
            currMaxH = charData.height;
        }
        else
        {
            // There's enough space for the next character, move the cursor along
            nextY = currY;
            nextX = currX + offset;
        }
        
        charData.uv.x( (float) currX / (float) TEXTURE_WIDTH_PX );
        charData.uv.y( (float) currY / (float) TEXTURE_HEIGHT_PX );
        charData.uv.w( (float) charData.width / (float) TEXTURE_WIDTH_PX );
        charData.uv.h( (float) charData.height / (float) TEXTURE_HEIGHT_PX );
        
        glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                currX,
                currY,
                charData.width,
                charData.height,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                m_face->glyph->bitmap.buffer
        );
        
        currX = nextX;
        currY = nextY;
    
        charVector.push_back( std::move(charData) );
    }
 
 
    m_currentTexOffset = {0, nextY + (int) currMaxH};
    m_charData.emplace(size, std::move(charVector) );
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
    
    for ( int i = 0; i < (int)openTags.size(); i++ )
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
    : m_texture(texture), m_renderObj(texture->handle()), m_charCount(0) {}

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
    
    m_size = {
        (int)(m_charBounds.back().x() + m_charBounds.back().w() - m_charBounds.front().x()),
        std::max(m_size.y(), (int) screenOffsets.h())
    };
}

void TextRenderObj::setPosition( Vector2f pos )
{
    for (int i = 0; i < m_charCount; i++ )
    {
        m_renderObj.setScreenVerts(i, pos.x() + m_charBounds[i].x(), pos.y() + m_charBounds[i].y(),
            m_charBounds[i].w(), m_charBounds[i].h());
    }
}

void TextRenderObj::setSize( Vector2i size )
{
    m_size = size;
}
