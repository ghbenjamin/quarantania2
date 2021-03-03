#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <utils/Containers.h>

// Forward definitions
struct SDL_Texture;
struct SDL_Surface;
class Texture;
class Surface;

// Typedefs
using TexturePtr = std::shared_ptr<Texture>;
using SurfacePtr = std::shared_ptr<Surface>;


class Texture
{
public:

    explicit Texture( SDL_Texture* texture );
    explicit Texture( std::shared_ptr<Surface> const& surface );
    ~Texture();

    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    const Vector2i& size() const&;
    const SDL_Texture* raw() const;
    SDL_Texture* raw();
    SDL_Rect& sourceRect();

    static TexturePtr loadTexture( std::string const& path );
    static TexturePtr createNewTexture( Vector2i size );

private:

    SDL_Texture* m_raw;
    Vector2i m_size;
    SDL_Rect m_sourceRect;
};



class Surface
{
public:
    explicit Surface( SDL_Surface* surface );
    ~Surface();

    Surface( const Surface& ) = delete;
    Surface& operator=( const Surface& ) = delete;

    SDL_Surface* raw();

    [[nodiscard]] static SurfacePtr createSurface( Vector2i size );

private:
    SDL_Surface* m_raw;
    Vector2i m_size;
};


class RawTexture
{
public:
    RawTexture( std::string const& path );
    ~RawTexture();
    
    RawTexture( const RawTexture& ) = delete;
    RawTexture& operator=( const RawTexture& ) = delete;

private:
    Vector2i m_size;
    unsigned char* m_data;
};