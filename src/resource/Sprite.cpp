#include <resource/Sprite.h>

#include <utils/Assert.h>

Sprite::Sprite()
{}

Sprite::Sprite(std::shared_ptr<Texture> const& texture, RectI const &region)
    : m_texture(texture)
{
    m_size = region.right();
    m_renderObj.handle = m_texture->handle();
    m_renderObj.screenBounds = {0, 0, region.w(), region.h()};
    
    // Convert our absolute texture bounds to fractional texture bounds
    float texX = (float)region.x() / (float)texture->size().x();
    float texY = (float)region.y() / (float)texture->size().y();
    float texW = (float)region.w() / (float)texture->size().x();
    float texH = (float)region.h() / (float)texture->size().y();
    
    // Calculate our fractional texture quad vertices

    // Top Left
    m_renderObj.verts[2] = texX;
    m_renderObj.verts[3] = texY + texH;

    // Bottom Right
    m_renderObj.verts[6] = texX + texW;
    m_renderObj.verts[7] = texY;

    // Bottom Left
    m_renderObj.verts[10] = texX;
    m_renderObj.verts[11] = texY;

    // Top Left
    m_renderObj.verts[14] = texX;
    m_renderObj.verts[15] = texY + texH;

    // Top Right
    m_renderObj.verts[18] = texX + texW;
    m_renderObj.verts[19] = texY + texH;

    // Bottom Left
    m_renderObj.verts[22] = texX + texW;
    m_renderObj.verts[23] = texY;
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
    : m_texture(texture)
{
    m_size = m_texture->size();
    m_renderObj.handle = m_texture->handle();
    m_renderObj.screenBounds = { 0, 0, m_size.x(), m_size.y() };

    // Top Left
    m_renderObj.verts[2] = 0.0f;
    m_renderObj.verts[3] = 1.0f;

    // Bottom Right
    m_renderObj.verts[6] = 1.0f;
    m_renderObj.verts[7] = 0.0f;

    // Bottom Left
    m_renderObj.verts[10] = 0.0f;
    m_renderObj.verts[11] = 0.0f;

    // Top Left
    m_renderObj.verts[14] = 0.0f;
    m_renderObj.verts[15] = 1.0f;

    // Top Right
    m_renderObj.verts[18] = 1.0f;
    m_renderObj.verts[19] = 1.0f;

    // Bottom Left
    m_renderObj.verts[22] = 1.0f;
    m_renderObj.verts[23] = 0.0f;
}

Sprite::operator bool() const
{
    return !!m_texture;
}

RenderObject Sprite::renderObject(Vector2i const &pos)
{
    Assert( !!m_texture );
    m_renderObj.screenBounds.x( pos.x() );
    m_renderObj.screenBounds.y( pos.y() );
    
    return m_renderObj;
}

void Sprite::setRenderLayer(RenderLayer layer)
{
    m_renderObj.renderLayer = layer;
}

Vector2i Sprite::size() const
{
    return m_size;
}

void Sprite::setTargetSize(Vector2i size)
{
    m_renderObj.screenBounds.w( size.x() );
    m_renderObj.screenBounds.h( size.y() );
}
