#pragma once

#include <graphics/Texture.h>

enum class RenderLayer
{
    Begin,

    Tiles,
    Entity,
    Actor,
    GFX,
    FOV,
    OVERLAY_UI,
    UI,

    End
};

struct RenderObject
{
    SDL_Texture* texture;       // The source SDL texture
    SDL_Rect sourceRect;        // The region of the source texture to copy from
    SDL_Rect targetRect;        // The region of the screen to copy to
    SDL_Rect clipRect;          // The region of the screen to clip to, if any
    RenderLayer renderLayer;    // The rendering layer this item should be grouped to

    RenderObject();
    RenderObject(SDL_Texture* texture, SDL_Rect src, SDL_Rect dst);
    ~RenderObject() = default;
};