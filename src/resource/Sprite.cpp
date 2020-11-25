#include <utility>


#include <resource/Sprite.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

Sprite::Sprite()
    : m_renderObj{ nullptr, {0, 0, 0, 0}, {0, 0, 0, 0}}
{
}

Sprite::Sprite(std::shared_ptr<Texture> texture, RectI const &region)
    : m_texture(std::move(texture)), m_renderObj{
        m_texture->raw(),
        region.toSDL(),
        {0, 0, region.w(), region.h()}
    }

{
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
    : m_texture(std::move(texture)), m_renderObj{ m_texture->raw(),{0, 0, 0, 0},{0, 0, 0, 0} }
{
    auto [w, h] = m_texture->size();

    m_renderObj.sourceRect.w = w;
    m_renderObj.sourceRect.h = h;
    m_renderObj.targetRect.w = w;
    m_renderObj.targetRect.h = h;
}

Sprite::operator bool() const
{
    return !!m_texture;
}

RenderObject Sprite::renderObject(Vector2i const &pos) const
{
    Assert( !!m_texture );

    m_renderObj.targetRect.x = pos.x();
    m_renderObj.targetRect.y = pos.y();

    return m_renderObj;
}

void Sprite::setRenderLayer(RenderLayer layer)
{
    m_renderObj.renderLayer = layer;
}

Vector2i Sprite::size() const
{
    if (m_renderObj.sourceRect.w != 0 || m_renderObj.sourceRect.h != 0)
    {
        return { m_renderObj.sourceRect.w, m_renderObj.sourceRect.h };
    }
    else
    {
        return textureSize();
    }
}

void Sprite::setClipRect(RectI rect)
{
    m_renderObj.clipRect = rect.toSDL();
}

Vector2i Sprite::textureSize() const
{
    return m_texture->size();
}
