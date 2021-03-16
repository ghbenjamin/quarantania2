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

std::shared_ptr<FtFontFace> FtFontManager::loadFontFace( std::string const &name, int height )
{
    std::string path = "../resource/font/" + name + ".ttf";
    
    FT_Face face;
    if (FT_New_Face(m_ft, path.c_str(), 0, &face))
    {
        AssertAlwaysMsg( "Unknown font: " + path );
    }
    
    FT_Set_Pixel_Sizes(face, 0, height);
    
    auto font = std::make_shared<FtFontFace>(face);
    auto pair = std::make_pair(path, height);
    m_fonts.emplace( pair, font );
    
    return font;
}

std::shared_ptr<FtFontFace> FtFontManager::getFont( std::string const &name, int size )
{
    auto pair = std::make_pair( name, size );
    auto it = m_fonts.find( pair );
    
    if (it != m_fonts.end())
    {
        return it->second;
    }
    else
    {
        return loadFontFace(name, size);
    }
}



// Font Face


FtFontFace::FtFontFace( FT_Face const& face )
    : m_face(face), m_currentTexOffset(0, 0)
{
    m_texture = std::make_shared<Texture>( Vector2i{ TEXTURE_SIZE_PX, 2 * TEXTURE_SIZE_PX } );
    generateFontData( 16 );
    generateFontData( 14 );
    generateFontData( 12 );
}

FtFontFace::~FtFontFace()
{
    FT_Done_Face(m_face);
}

CompositeSprite FtFontFace::renderString( std::string const &str, int fontSize )
{
    auto it = m_charData.find(fontSize);
    AssertMsg( it != m_charData.end(), "Missing font size" );
    auto& chardata = it->second;
    
    int currX = 0;
    int currY = 0;
    
    for (int i = 0; i < (int) str.size(); i++)
    {
        std::uint16_t char_idx = static_cast<std::uint16_t>( str.at(i) );
        
        
    }

    return CompositeSprite();
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
    int currMaxH = 0;
    
    for (int ch = 0; ch < GLYPH_RANGE_MAX; ch++)
    {
        if (FT_Load_Char(m_face, ch, FT_LOAD_RENDER))
        {
            AssertAlways();
        }
        
        FtCharData charData;
        
        charData.width = m_face->glyph->bitmap.width;
        charData.height = m_face->glyph->bitmap.rows;
        charData.bearingX = m_face->glyph->bitmap_left;
        charData.bearingY = m_face->glyph->bitmap_top;
        charData.advance = m_face->glyph->advance.x;
        
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
        
        charData.uvX = (float) currX / (float) TEXTURE_SIZE_PX;
        charData.uvY = (float) currY / (float) TEXTURE_SIZE_PX;
        
        
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
 
 
    m_currentTexOffset = {0, nextY + currMaxH};
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
