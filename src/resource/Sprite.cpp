#include <resource/Sprite.h>

#include <utils/Assert.h>

Sprite::Sprite()
    : m_renderObj(0), m_colour(Colour::White)
{}

Sprite::Sprite(std::shared_ptr<Texture> const& texture, RectI const &region)
    : m_texture(texture),m_renderObj( m_texture->handle() ), m_colour(Colour::White)
{
    m_screenBounds.setRight( region.right() );
    auto regionf = region.convert<float>();
    auto sizef = texture->size().convert<float>();
    
    m_renderObj.setScreenVerts( 0, 0, 0, regionf.w(), regionf.h() );
    
    // Convert our absolute texture bounds to fractional texture bounds
    float texX = regionf.x() / sizef.x();
    float texY = regionf.y() / sizef.y();
    float texW = regionf.w() / sizef.x();
    float texH = regionf.h() / sizef.y();
    
    m_renderObj.setTextureVerts( 0, texX, texY, texW, texH );
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
    : m_texture(texture), m_renderObj( m_texture->handle() ), m_colour(Colour::White)
{
    m_screenBounds.setRight( m_texture->size() );
    
    m_renderObj.setScreenVerts( 0, 0.0f, 0.0f, (float)m_screenBounds.w(), (float)m_screenBounds.h() );
    m_renderObj.setTextureVerts( 0, 0.0f, 0.0f, 1.0f, 1.0f );
}

Sprite::operator bool() const
{
    return !!m_texture;
}

RenderObject Sprite::renderObject(Vector2i const &pos)
{
    Assert( !!m_texture );
    setPosition( pos );
    return m_renderObj;
}

void Sprite::setRenderLayer(RenderLayer layer)
{
    m_renderLayer = layer;
}

Vector2i Sprite::size() const
{
    return m_screenBounds.right();
}

void Sprite::setTargetSize(Vector2i size)
{
    m_screenBounds.setRight( size );
    m_renderObj.setScreenVerts( 0, (float) m_screenBounds.x(), (float) m_screenBounds.y(),
        (float) size.x(), (float) size.y() );
}

void Sprite::setPosition( Vector2i position )
{
    m_screenBounds.setLeft( position );
    m_renderObj.setScreenVerts( 0, (float) position.x(), (float) position.y(),
        (float) m_screenBounds.w(), (float) m_screenBounds.h() );
}

void Sprite::setColour( Colour colour )
{
    auto colourGl = colour.asOpenGL();
    m_renderObj.setColourVerts( 0, colourGl[0], colourGl[1], colourGl[2], colourGl[3] );
}

Colour const &Sprite::getColour()
{
    return m_colour;
}


// Composite Sprite
// -------------------------



CompositeSprite::CompositeSprite()
    : m_renderObj({}), m_texture(0), m_size(0, 0), m_itemCount(1) {}

CompositeSprite::CompositeSprite( TexturePtr texture )
    : m_renderObj({}), m_texture(texture), m_size(0, 0), m_itemCount(1) {}

void CompositeSprite::addQuad( RectF screenOffsets, RectF uvBounds )
{
    if (m_itemCount == 1)
    {
        m_renderObj.setScreenVerts(0, screenOffsets.x(), screenOffsets.y(), screenOffsets.w(), screenOffsets.h() );
        m_renderObj.setTextureVerts(0, uvBounds.x(), uvBounds.y(), uvBounds.w(), uvBounds.h() );
    }
    else
    {
        m_renderObj.addQuad( screenOffsets, uvBounds );
        m_itemCount++;
    }
    
    m_screenBounds.push_back( screenOffsets );
    
    m_size = {
        m_size.x() + (int)screenOffsets.w(),
        std::max(m_size.y(), (int)screenOffsets.h())
    };
}

RenderObject CompositeSprite::renderObject( Vector2i const &pos )
{
    for (int i = 0; i < m_itemCount; i++ )
    {
        m_renderObj.setScreenVerts(i, pos.x() + m_screenBounds[i].x(), pos.y() + m_screenBounds[i].y(),
            m_screenBounds[i].w(), m_screenBounds[i].h() );
    }
    
    return m_renderObj;
}

Vector2i CompositeSprite::size() const
{
    return m_size;
}
