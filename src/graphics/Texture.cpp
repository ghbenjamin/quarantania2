
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
    
    bind();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x(), m_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    unbind();
    stbi_image_free(data);
}

Texture::Texture( GLuint handle, Vector2i size )
        : m_handle(handle), m_size(size) {}

Texture::Texture( Vector2i size )
:   m_size(size)
{
    glGenTextures( 1, &m_handle );
    
    bind();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x(), m_size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    unbind();
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

void Texture::dumpToImage( std::string const& path )
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_handle, 0);

    int data_size = m_size.x() * m_size.y() * 4;
    GLubyte* pixels = new GLubyte[data_size];
    glReadPixels(0, 0, m_size.x(), m_size.y(), GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);

    stbi_write_bmp( path.c_str(), m_size.x(), m_size.y(), 4, pixels );
    delete[] pixels;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


Surface::Surface()
: m_surface(nullptr), m_size{-1, -1}, m_format(PixelFormat::RGBA) {}

Surface::Surface( std::string const &path ) : m_format(PixelFormat::RGBA)
{
    int format = STBI_rgb_alpha;
    int width, height, origFormat;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &origFormat, format);
    
    if ( data == nullptr )
    {
        AssertAlwaysMsg( fmt::format( "Loading image {} failed: {}", path, stbi_failure_reason() ) );
    }
    
    m_size = {width, height};
    
    std::uint32_t pixel_format = SDL_PIXELFORMAT_RGBA32;
    int depth = 32;
    int pitch = 4 * width;
    
    SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height, depth, pitch, pixel_format);
    m_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    
    SDL_BlitSurface( temp, NULL, m_surface, NULL );
    
    if ( m_surface == NULL )
    {
        stbi_image_free(data);
        AssertAlwaysMsg( fmt::format( "Error creating SDL surface from stbi data", SDL_GetError() ) );
    }
    
    SDL_FreeSurface(temp);
    stbi_image_free(data);
}


Surface::Surface(SDL_Surface *surface, PixelFormat format)
    : m_surface(surface), m_format(format)
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

Surface::Surface( Vector2i size )
        : m_size(size), m_format(PixelFormat::RGBA)
{
    m_surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x(), size.y(), 32, SDL_PIXELFORMAT_RGBA32 );
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
    
    SDL_Surface* target = m_surface;
    SDL_Surface* converted = nullptr;
    
    // SDL_TTF gives surfaces in ARGB format, so we need to convert to RGBA for OpenGL
    if (m_format == PixelFormat::ARGB)
    {
        converted = SDL_ConvertSurfaceFormat(m_surface, SDL_PIXELFORMAT_ABGR8888, 0);
        if (converted == nullptr)
        {
            AssertAlwaysMsg( "Failed to convert SDL_Surface from ARBG to RBGA" );
        }
        
        target = converted;
    }
    
    
    GLuint handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, target->w, target->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, target->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);


    if (converted != nullptr)
    {
        SDL_FreeSurface(converted);
    }

    return std::make_shared<Texture>( handle, m_size );
}



SDL_Surface *Surface::raw()
{
    return m_surface;
}

