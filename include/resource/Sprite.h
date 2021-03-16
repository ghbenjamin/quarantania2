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
    Colour const& getColour();
    
    RenderObject renderObject(Vector2i const& pos);
    
    void setColour( Colour colour );
    void setTargetSize( Vector2i size );
    void setRenderLayer(RenderLayer layer);
    
private:
    RenderLayer m_renderLayer;
    Colour m_colour;
    TexturePtr m_texture;
    Vector2i m_size;
    RenderObject m_renderObj;
};


class CompositeSprite
{
public:
    CompositeSprite() = default;
    ~CompositeSprite() = default;

private:
    std::size_t m_itemCount;
    Vector2i m_size;
    TexturePtr m_texture;
    
    std::vector<Colour> m_colours;
    std::vector<RenderObject> m_renderObjects;
};