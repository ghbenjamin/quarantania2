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

class Sprite;

struct RenderObject
{
    friend class Sprite;
    ~RenderObject() = default;
    
    TextureHandle handle;
    RenderLayer renderLayer;
    
    GLfloat verts[24];
    RectI screenBounds;
    
// Don't try and make render objects outside of sprites
private:
    RenderObject();
};