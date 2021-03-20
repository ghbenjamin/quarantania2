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
    Sprite(RenderObject const& obj, Vector2i size);
    ~Sprite() = default;

    explicit operator bool() const;
    Vector2i size() const;
    Colour const& getColour();
    
    RenderObject renderObject(Vector2i const& pos);
    
    void setPosition( Vector2i position );
    void setTargetSize( Vector2i size );
    void setColour( Colour colour );
    void setRenderLayer(RenderLayer layer);
    
private:
    Colour m_colour;
    RenderLayer m_renderLayer;
    TexturePtr m_texture;
    RenderObject m_renderObj;
    RectI m_screenBounds;
};