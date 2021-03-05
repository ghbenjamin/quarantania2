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
    RenderObject();
    ~RenderObject() = default;
    
    TextureHandle handle;
    RenderLayer renderLayer;
    
    GLfloat verts[24];
    RectI screenBounds;
};