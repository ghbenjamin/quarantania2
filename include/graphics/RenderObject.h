#pragma once

#include <resource/Texture.h>

enum class RenderLayer
{
    Begin,

    Tiles,
    Entity,
    Actor,
    GFX,
    FOV,
    UI,

    End
};

struct RenderObject
{
    SDL_Texture* texture;
    SDL_Rect sourceRect;
    SDL_Rect targetRect;
    RenderLayer renderLayer;

    RenderObject();
    RenderObject(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);
    ~RenderObject() = default;
};