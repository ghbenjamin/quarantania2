#pragma once

#include <resource/Texture.h>

struct RenderObject
{
    TexturePtr texture;
    RectI sourceRect;
    RectI targetRect;
};