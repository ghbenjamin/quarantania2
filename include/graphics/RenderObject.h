#pragma once

#include <resource/Texture.h>

struct RenderObject
{
    SDL_Texture* texture;
    SDL_Rect sourceRect;
    SDL_Rect targetRect;

    RenderObject()
        : texture(nullptr), sourceRect{0, 0, 0, 0}, targetRect{0, 0, 0, 0} {}
    RenderObject(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst )
        : texture(texture), sourceRect(src), targetRect(dst) {}

    ~RenderObject() = default;

};