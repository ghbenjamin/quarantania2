
#include <SDL2/SDL_image.h>

#ifdef USE_GL
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <graphics/Texture.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>



//Texture::Texture(SDL_Texture* texture)
//: m_raw(texture)
//{
//    if (m_raw)
//    {
//        int w, h;
//        SDL_QueryTexture( m_raw, nullptr, nullptr, &w, &h );
//        m_size = Vector2i{w, h};
//        m_sourceRect = {0, 0, w, h};
//    }
//}
//
//Texture::Texture( std::shared_ptr<Surface>const& surface)
//    : Texture( SDL_CreateTextureFromSurface(
//            ResourceManager::get().getWindow()->renderer(),
//            surface->raw()
//        )) {}
//
//Texture::~Texture()
//{
//    if (m_raw != nullptr)
//    {
//        SDL_DestroyTexture(m_raw);
//        m_raw = nullptr;
//    }
//}
//
//const Vector2i &Texture::size() const &
//{
//    return m_size;
//}
//
//const SDL_Texture *Texture::raw() const
//{
//    return m_raw;
//}
//
//SDL_Texture *Texture::raw()
//{
//    return m_raw;
//}
//
//TexturePtr Texture::loadTexture(std::string const &path)
//{
//    SDL_Texture* text = IMG_LoadTexture(
//        ResourceManager::get().getWindow()->renderer(),
//        path.c_str()
//    );
//
//    return std::make_shared<Texture>(text);
//}
//
//SDL_Rect &Texture::sourceRect()
//{
//    return m_sourceRect;
//}
//
//TexturePtr Texture::createNewTexture(Vector2i size)
//{
//    SDL_Texture* texture = SDL_CreateTexture(
//        ResourceManager::get().getWindow()->renderer(),
//        SDL_PIXELFORMAT_ABGR8888,
//        SDL_TEXTUREACCESS_STATIC,
//        size.x(),
//        size.y()
//    );
//
//    return std::make_shared<Texture>(texture);
//}
//
//Surface::Surface(SDL_Surface *surface)
//    : m_raw(surface)
//{
//    if (m_raw != nullptr)
//    {
//        m_size = Vector2i{ m_raw->w, m_raw->h };
//    }
//    else
//    {
//        m_size = { -1, -1 };
//    }
//}
//
//Surface::~Surface()
//{
//    if (m_raw != nullptr)
//    {
//        SDL_FreeSurface(m_raw);
//    }
//}
//
//SurfacePtr Surface::createSurface( Vector2i size )
//{
//    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x(), size.y(), 32, SDL_PIXELFORMAT_RGBA32 );
//    AssertMsg( surface != nullptr, fmt::format("Failed to create surface: {}", SDL_GetError() ) );
//
//    return std::make_shared<Surface>(surface);
//}
//
//SDL_Surface *Surface::raw()
//{
//    return m_raw;
//}


#ifdef USE_GL
Texture::Texture( std::string const &path )
{
    int format = STBI_rgb_alpha;
    int width, height, origFormat;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &origFormat, format);
    
    if ( data == nullptr )
    {
        AssertAlwaysMsg( fmt::format( "Loading image {} failed: {}", path, stbi_failure_reason() ) );
    }
    
    m_size = {width, height};
    
    glGenTextures( 1, &m_handle );
    glBindTexture( GL_TEXTURE_2D, m_handle );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x(), m_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_handle);
}


GLuint Texture::handle() const
{
    return m_handle;
}

Vector2i Texture::size() const
{
    return m_size;
}

Texture::Texture( GLuint handle, Vector2i size )
: m_handle(handle), m_size(size)
{
}

#endif

Surface::Surface()
: m_surface(nullptr), m_size{-1, -1}
{

}

Surface::Surface(SDL_Surface *surface)
    : m_surface(surface)
{
    if (m_surface != nullptr)
    {
        m_size = Vector2i{ m_surface->w, m_surface->h };
    }
    else
    {
        m_size = { -1, -1 };
    }
}

Surface::~Surface()
{
    if (m_surface != nullptr)
    {
        SDL_FreeSurface(m_surface);
    }
}

std::shared_ptr<Texture> Surface::toTexture()
{
    Uint8 colours = m_surface->format->BytesPerPixel;
    GLuint textureFormat;
    
    if (colours == 4)
    {
        if (m_surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
        else
            textureFormat = GL_BGRA;
    }
    else
    {
        if (m_surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGB;
        else
            textureFormat = GL_BGR;
    }
    
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, colours, m_surface->w, m_surface->h, 0,
                 textureFormat, GL_UNSIGNED_BYTE, m_surface->pixels);

    return std::make_shared<Texture>( handle, m_size );
}

Surface::Surface( Vector2i size )
    : m_size(size)
{
    m_surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x(), size.y(), 32, SDL_PIXELFORMAT_RGBA32 );
}

SDL_Surface *Surface::raw()
{
    return m_surface;
}
