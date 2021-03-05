#pragma once

#include <graphics/Texture.h>
#include <graphics/RenderObject.h>


class Sprite
{
public:

    Sprite();
    Sprite(TexturePtr const& texture, RectI const& region);
    Sprite(TexturePtr texture);
    ~Sprite() = default;

    explicit operator bool() const;
    Vector2i size() const;
    RenderObject renderObject(Vector2i const& pos);
    
    void setTargetSize( Vector2i size );
    void setRenderLayer(RenderLayer layer);
    
private:
    TexturePtr m_texture;
    Vector2i m_size;
    float m_texVerts[8];

    RenderObject m_renderObj;
};