#pragma once

#include <graphics/Texture.h>
#include <graphics/RenderObject.h>
#include <utils/Colour.h>


class Sprite
{
public:

    Sprite();   // Empty sprite
    Sprite(TexturePtr const& texture, RectI const& region); // Subregion of texture
    Sprite(TexturePtr texture);     // Entire textire
    Sprite(RenderObject const& obj, Vector2i size); // From existing renderobject
    ~Sprite() = default;

    explicit operator bool() const;
    Vector2i size() const;
    Colour const& getColour();
    
    RenderObject renderObject(Vector2i const& pos);
    
    void setPosition( Vector2i position );
    void setTargetSize( Vector2i size );
    
    void setColourMod( Colour colour );
    void resetColourMod();
    void setPermanentColour( Colour colour );
    
    void setAlphaMod( float alpha );
    void resetAlphaMod();

private:
    Colour m_colour;
    TexturePtr m_texture;
    RenderObject m_renderObj;
    RectI m_screenBounds;
};