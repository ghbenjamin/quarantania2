#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <utils/Containers.h>

// Forward definitions
struct SDL_Texture;
class Texture;

// Typedefs
using TexturePtr = std::shared_ptr<Texture>;

class Texture
{
public:

    explicit Texture( SDL_Texture* t );
    Texture( Vector2i size );
    virtual ~Texture();

    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    const Vector2i& size() const&;
    const SDL_Texture* raw() const;
    SDL_Texture* raw();

    SDL_Rect& sourceRect();

    static TexturePtr loadTexture( std::string const& path );


private:

    SDL_Texture* m_raw;
    Vector2i m_size;
    SDL_Rect m_sourceRect;
};

using TexturePtr = std::shared_ptr<Texture>;