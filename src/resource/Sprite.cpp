#include <resource/Sprite.h>

#include <utils/Assert.h>

Sprite::Sprite()
    : m_renderObj(0), m_colour(Colour::White)
{}

Sprite::Sprite(std::shared_ptr<Texture> const& texture, RectI const &region)
    : m_texture(texture),m_renderObj( m_texture->handle() ), m_colour(Colour::White)
{
    m_size = region.right();
    auto regionf = region.convert<float>();
    auto sizef = texture->size().convert<float>();
    
    m_renderObj.setScreenVerts( 0, 0, regionf.w(), regionf.h() );
    
    // Convert our absolute texture bounds to fractional texture bounds
    float texX = regionf.x() / sizef.x();
    float texY = regionf.y() / sizef.y();
    float texW = regionf.w() / sizef.x();
    float texH = regionf.h() / sizef.y();
    
    m_renderObj.setTextureVerts( texX, texY, texW, texH );
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
    : m_texture(texture), m_renderObj( m_texture->handle() ), m_colour(Colour::White)
{
    m_size = m_texture->size();
    
    m_renderObj.setScreenVerts( 0.0f, 0.0f, (float)m_size.x(), (float)m_size.y() );
    m_renderObj.setTextureVerts( 0.0f, 0.0f, 1.0f, 1.0f );
}

Sprite::operator bool() const
{
    return !!m_texture;
}

RenderObject Sprite::renderObject(Vector2i const &pos)
{
    Assert( !!m_texture );
    m_renderObj.setScreenPosition( pos.convert<float>() );
    return m_renderObj;
}

void Sprite::setRenderLayer(RenderLayer layer)
{
    m_renderObj.setRenderLayer( layer );
}

Vector2i Sprite::size() const
{
    return m_size;
}

void Sprite::setTargetSize(Vector2i size)
{
    m_renderObj.setScreenSize( size.convert<float>() );
    m_size = size;
}

void Sprite::setColour( Colour colour )
{
    auto colourGl = colour.asOpenGL();
    m_renderObj.setColourVerts( colourGl[0], colourGl[1], colourGl[2], colourGl[3] );
}

Colour const &Sprite::getColour()
{
    return m_colour;
}
