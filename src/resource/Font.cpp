#include <resource/Font.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <utils/Assert.h>

Font::Font( TTF_Font* ptr )
: m_font(ptr)
{
}

Font::~Font()
{
    if (m_font)
    {
        TTF_CloseFont(m_font);
    }
}

FontPtr Font::loadFont(std::string const& path, int size)
{
    auto font = TTF_OpenFont( path.c_str(), size );
    AssertMsg( font, TTF_GetError() );
    return std::make_shared<Font>(font);
}
