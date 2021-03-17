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


class CompositeSprite
{
public:
    CompositeSprite();
    CompositeSprite(TexturePtr texture);
    ~CompositeSprite() = default;
    
    Vector2i size() const;
    RenderObject renderObject(Vector2i const& pos);
    
    void addQuad( RectF screenOffsets, RectF uvBounds );

private:
    std::size_t m_itemCount;
    Vector2i m_size;
    TexturePtr m_texture;
    
    std::vector<Colour> m_colours;
    std::vector<RectF> m_screenBounds;
    RenderObject m_renderObj;
};