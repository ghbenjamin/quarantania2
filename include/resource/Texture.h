#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <SDL2/SDL.h>

#include <utils/Containers.h>

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture
{
public:

    explicit Texture( SDL_Texture* );
    virtual ~Texture();

    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    const Vector2i& size() const&;
    const SDL_Texture* raw() const;

    static TexturePtr loadTexture( SDL_Renderer* renderer, std::string const& path );


private:

    SDL_Texture* m_raw;
    Vector2i m_size;
};

using TexturePtr = std::shared_ptr<Texture>;