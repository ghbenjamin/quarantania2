
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include <resource/Texture.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>

Texture::Texture(SDL_Texture* t)
: m_raw(t)
{
    if (m_raw)
    {
        int w, h;
        SDL_QueryTexture( m_raw, nullptr, nullptr, &w, &h );
        m_size = Vector2i{w, h};
        m_sourceRect = {0, 0, w, h};
    }
}


Texture::Texture(Vector2i size)
: m_size(size), m_sourceRect{0, 0, size.x(), size.y()}
{
    m_raw = SDL_CreateTexture(
        ResourceManager::get().getWindow()->renderer()->raw(),
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STATIC,
        size.x(),
        size.y()
    );
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

SDL_Texture *Texture::raw()
{
    return m_raw;
}

TexturePtr Texture::loadTexture(std::string const &path)
{
    SDL_Surface* image = IMG_Load( path.c_str() );
    AssertMsg( image != nullptr, "Expected loaded surface" );

    auto texture = SDL_CreateTextureFromSurface( ResourceManager::get().getWindow()->renderer()->raw(), image );
    AssertMsg(texture != nullptr, "Expected loaded texture");

    SDL_FreeSurface( image );

    auto ptr = std::make_shared<Texture>(texture);
    return ptr;
}

SDL_Rect &Texture::sourceRect()
{
    return m_sourceRect;
}
