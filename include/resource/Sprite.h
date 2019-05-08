#pragma once

#include <resource/Texture.h>
#include <graphics/RenderObject.h>


class Sprite
{
public:

    Sprite();
    Sprite(std::shared_ptr<Texture> const& texture, RectI const& region);
    explicit Sprite(std::shared_ptr<Texture> const& texture );

    virtual ~Sprite() = default;

    explicit operator bool();

    const RenderObject renderObject( RectI const& target ) const;

private:

    std::shared_ptr<Texture> m_texture;
    RectI m_region;

};