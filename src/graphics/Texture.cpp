
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <graphics/Texture.h>
#include <utils/Assert.h>
#include <utils/Logging.h>



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

Texture::Texture( GLuint handle, Vector2i size )
        : m_handle(handle), m_size(size)
{
    int foo = 3;
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, colours, m_surface->w, m_surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, m_surface->pixels);
    glBindTexture( GL_TEXTURE_2D, 0 );

    return std::make_shared<Texture>( handle, m_size );
}

Surface::Surface( Vector2i size )
    : m_size(size)
{
    m_surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x(), size.y(), 32, SDL_PIXELFORMAT_ABGR32 );

//    SDL_SetSurfaceAlphaMod(m_surface, 0xFF);
//    SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_NONE);
}

SDL_Surface *Surface::raw()
{
    return m_surface;
}
