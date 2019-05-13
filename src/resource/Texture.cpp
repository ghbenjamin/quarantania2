
#include <resource/Texture.h>
#include <SDL2/SDL_image.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

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
    if (m_raw != nullptr)
    {
        SDL_DestroyTexture(m_raw);
        m_raw = nullptr;
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

TexturePtr Texture::loadTexture(SDL_Renderer *renderer, std::string const &path)
{
    SDL_Surface* image = IMG_Load( path.c_str() );
    AssertMsg( image != nullptr, "Expected loaded surface" );

    auto texture = SDL_CreateTextureFromSurface( renderer, image );
    AssertMsg( texture != nullptr, "Expected loaded texture");

    SDL_FreeSurface( image );

    auto ptr = std::make_shared<Texture>(texture);
    return ptr;
}