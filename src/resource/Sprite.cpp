#include <resource/Sprite.h>
#include <utils/Assert.h>

Sprite::Sprite()
: m_texture(nullptr), m_textureRegion{0, 0, 0, 0}, m_size{0, 0}
{
}

Sprite::Sprite(std::shared_ptr<Texture> const &texture, RectI const &region)
    : m_texture(texture), m_size(region.right()), m_textureRegion( region.toSDL() )
{
}

Sprite::Sprite(std::shared_ptr<Texture> const &texture)
    : m_texture(texture), m_size(m_texture->size()), m_textureRegion{0, 0, m_size.x(), m_size.y() }
{
}

Sprite::operator bool()
{
    return !!m_texture;
}

const RenderObject Sprite::renderObject(Vector2i const &pos) const
{
    Assert( !!m_texture );
    return {
        const_cast<SDL_Texture *>(m_texture->raw()),
        m_textureRegion,
        SDL_Rect{ pos.x(), pos.y(), m_size.x(), m_size.y() }
    };
}
