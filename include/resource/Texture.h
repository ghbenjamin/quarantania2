#pragma once

#include <string>
#include <string_view>
#include <memory>

#include <SDL2/SDL.h>

#include <utils/Containers.h>

class Texture
{
public:

    Texture();
    explicit Texture( SDL_Texture* );
    virtual ~Texture();

    Texture( const Texture& ) = delete;
    Texture& operator=( const Texture& ) = delete;

    const Vector2i& size() const&;
    const SDL_Texture* raw() const;

private:

    SDL_Texture* m_raw;
    Vector2i m_size;
};

using TexturePtr = std::shared_ptr<Texture>;


namespace IO
{
    TexturePtr loadTexture( SDL_Renderer* renderer, std::string_view path );
}