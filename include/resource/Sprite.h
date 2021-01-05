#pragma once

#include <graphics/Texture.h>
#include <graphics/RenderObject.h>


class Sprite
{
public:

    Sprite();
    Sprite(TexturePtr texture, RectI const& region);
    explicit Sprite(TexturePtr texture);
    virtual ~Sprite() = default;

    // Which layer of render objects should this item be grouped with?
    void setRenderLayer(RenderLayer layer);

    // Should this item be clipped by another rectangle?
    void setClipRect( RectI rect );

    [[nodiscard]] Vector2i size() const;
    [[nodiscard]] Vector2i textureSize() const;
    [[nodiscard]] explicit operator bool() const;
    [[nodiscard]] RenderObject renderObject(Vector2i const& pos) const;

private:

    TexturePtr m_texture;
    mutable RenderObject m_renderObj;
};