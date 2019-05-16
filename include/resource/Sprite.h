#pragma once

#include <resource/Texture.h>
#include <graphics/RenderObject.h>


class Sprite
{
public:

    Sprite();
    Sprite(TexturePtr const& texture, RectI const& region);
    explicit Sprite(TexturePtr const& texture);
    virtual ~Sprite() = default;

    explicit operator bool();

    const RenderObject renderObject(Vector2i const& pos) const;

private:

    TexturePtr m_texture;
    Vector2i m_size;
    SDL_Rect m_textureRegion;

};