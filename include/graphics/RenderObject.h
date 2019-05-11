#pragma once

#include <resource/Texture.h>

struct RenderObject
{
    SDL_Texture* texture;
    SDL_Rect* sourceRect;

    SDL_Rect* targetRect;
};