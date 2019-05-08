
#include <resource/Texture.h>
#include <SDL2/SDL_image.h>
#include <utils/Assert.h>


Texture::Texture()
: m_raw(nullptr), m_size( Vector2i::Null() )
{
    m_raw = nullptr;
}

Texture::Texture(SDL_Texture* t)
{
    m_raw = t;

    if (m_raw)
    {
        int w, h;
        SDL_QueryTexture( m_raw, nullptr, nullptr, &w, &h );
        m_size = Vector2i{w, h};
    }
}

Texture::~Texture()
{
    if (m_raw)
    {
        SDL_DestroyTexture(m_raw);
    }
}

const Vector2i &Texture::size() const &
{
    return m_size;
}

const SDL_Texture *Texture::raw() const
{
    return m_raw;
}

TexturePtr IO::loadTexture( SDL_Renderer* renderer, std::string_view path )
{
    SDL_Surface* image = IMG_Load( std::string(path).c_str() );
    AssertMsg( image != nullptr, "Expected loaded surface" );

    auto texture = SDL_CreateTextureFromSurface( renderer, image );
    AssertMsg( texture != nullptr, "Expected loaded texture");

    SDL_FreeSurface( image );

    return std::make_shared<Texture>(texture);
}
