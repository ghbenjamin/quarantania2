
#include <SDL2/SDL_image.h>
#include <stb_image.h>

#include <graphics/Texture.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>



Texture::Texture(SDL_Texture* texture)
: m_raw(texture)
{
    if (m_raw)
    {
        int w, h;
        SDL_QueryTexture( m_raw, nullptr, nullptr, &w, &h );
        m_size = Vector2i{w, h};
        m_sourceRect = {0, 0, w, h};
    }
}

Texture::Texture( std::shared_ptr<Surface>const& surface)
    : Texture( SDL_CreateTextureFromSurface(
            ResourceManager::get().getWindow()->renderer(),
            surface->raw()
        )) {}

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
    SDL_Texture* text = IMG_LoadTexture(
        ResourceManager::get().getWindow()->renderer(),
        path.c_str()
    );

    return std::make_shared<Texture>(text);
}

SDL_Rect &Texture::sourceRect()
{
    return m_sourceRect;
}

TexturePtr Texture::createNewTexture(Vector2i size)
{
    SDL_Texture* texture = SDL_CreateTexture(
        ResourceManager::get().getWindow()->renderer(),
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STATIC,
        size.x(),
        size.y()
    );

    return std::make_shared<Texture>(texture);
}

Surface::Surface(SDL_Surface *surface)
    : m_raw(surface)
{
    if (m_raw != nullptr)
    {
        m_size = Vector2i{ m_raw->w, m_raw->h };
    }
    else
    {
        m_size = { -1, -1 };
    }
}

Surface::~Surface()
{
    if (m_raw != nullptr)
    {
        SDL_FreeSurface(m_raw);
    }
}

SurfacePtr Surface::createSurface( Vector2i size )
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x(), size.y(), 32, SDL_PIXELFORMAT_RGBA32 );
    AssertMsg( surface != nullptr, fmt::format("Failed to create surface: {}", SDL_GetError() ) );

    return std::make_shared<Surface>(surface);
}

SDL_Surface *Surface::raw()
{
    return m_raw;
}

RawTexture::RawTexture( std::string const &path )
{
    int format = STBI_rgb_alpha;
    int width, height, origFormat;
    m_data = stbi_load("./test.png", &width, &height, &origFormat, format);
    
    if ( m_data == nullptr )
    {
        AssertAlwaysMsg( fmt::format( "Loading image {} failed: {}", path, stbi_failure_reason() ) );
    }
    
    m_size = {width, height};
}

RawTexture::~RawTexture()
{
    if (m_data != nullptr)
    {
        stbi_image_free(m_data);
    }
}
