#pragma once

#include <graphics/Texture.h>
#include <graphics/RenderObject.h>
#include <utils/Colour.h>


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
    
    void setColour( Colour colour );
    void setTargetSize( Vector2i size );
    void setRenderLayer(RenderLayer layer);
    
private:

    Colour m_colour;
    TexturePtr m_texture;
    Vector2i m_size;
    RenderObject m_renderObj;
};
