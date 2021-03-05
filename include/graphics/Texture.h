#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <glad/glad.h>

#include <utils/Containers.h>



class Texture
{
public:
    Texture( std::string const& path );
    Texture( GLuint handle, Vector2i size );
    ~Texture();
    
    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    GLuint handle() const;
    Vector2i size() const;
    
private:
    GLuint m_handle;
    Vector2i m_size;
};


class Surface
{
public:
    Surface();
    Surface(SDL_Surface* surface);
    Surface(Vector2i size);
    ~Surface();

    std::shared_ptr<Texture> toTexture();
    SDL_Surface* raw();

private:
    
    Vector2i m_size;
    SDL_Surface* m_surface;
};


using TexturePtr = std::shared_ptr<Texture>;
using TextureHandle = GLuint;
