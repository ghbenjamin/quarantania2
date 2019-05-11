#include <resource/Sprite.h>

Sprite::Sprite()
    : m_texture(nullptr), m_region( RectI::Null() )
{
}

Sprite::Sprite(std::shared_ptr<Texture> const &texture, RectI const &region)
    : m_texture(texture), m_region( region )
{
}

Sprite::Sprite(std::shared_ptr<Texture> const &texture)
    : m_texture(texture), m_region( { {0, 0}, texture->size() } )
{
}

Sprite::operator bool()
{
    return !!m_texture;
}

const RenderObject Sprite::renderObject(const RectI & target) const
{
//    return {
//        m_texture,
//        m_region,
//        target
//    };
}


