#pragma once

#include <resource/Texture.h>
#include <graphics/RenderObject.h>


class Sprite
{
public:

    Sprite();
    Sprite(TexturePtr  texture, RectI const& region);
    explicit Sprite(TexturePtr  texture);
    virtual ~Sprite() = default;

    explicit operator bool();

    RenderObject renderObject(Vector2i const& pos) const;

private:

    TexturePtr m_texture;
    mutable RenderObject m_renderObj;
};