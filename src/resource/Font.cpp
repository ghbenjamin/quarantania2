#include <resource/Font.h>

#include <string>

#include <SDL2/SDL_ttf.h>

#include <utils/Assert.h>
#include <resource/Texture.h>
#include <resource/ResourceManager.h>

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

std::shared_ptr<Texture> Font::renderText(std::string const &text, Colour colour, int wrapWidth)
{
    SDL_Surface* surface;

    if (wrapWidth > 0)
    {
        surface = TTF_RenderText_Blended_Wrapped( m_font, text.c_str(), colour.raw(), wrapWidth );
    }
    else
    {
        surface = TTF_RenderText_Blended( m_font, text.c_str(), colour.raw() );
    }

    Assert(surface != nullptr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        ResourceManager::get().getWindow()->renderer()->raw(), surface );

    Assert(texture != nullptr);

    SDL_FreeSurface(surface);

    return std::make_shared<Texture>(texture);
}
