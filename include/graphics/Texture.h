#pragma once

#include <string>
#include <string_view>
#include <memory>

#pragma warning (disable : 4005)
#include <glad/glad.h>

#include <utils/Containers.h>


enum class PixelFormat
{
    RGBA,
    ARGB
};


class Texture
{
public:
    // Create a texture from an image file
    Texture( std::string const& path );
    
    // Take ownership of a texture already in memory
    Texture( GLuint handle, Vector2i size );
    
    // Create a new empty texture of the given size
    Texture( Vector2i size );
    
    ~Texture();
    
    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    GLuint handle() const;
    Vector2i size() const;
    
    void bind();
    void unbind();

    void dumpToImage( std::string const& path );
    
private:
    GLuint m_handle;
    Vector2i m_size;
};


class Surface
{
public:
    Surface();
    Surface( std::string const& path );
    Surface(SDL_Surface* surface, PixelFormat format);
    Surface(Vector2i size);
    ~Surface();

    std::shared_ptr<Texture> toTexture();
    SDL_Surface* raw();

private:
    Vector2i m_size;
    SDL_Surface* m_surface;
    PixelFormat m_format;
};


using SurfacePtr = std::shared_ptr<Surface>;
using TexturePtr = std::shared_ptr<Texture>;
using TextureHandle = GLuint;
