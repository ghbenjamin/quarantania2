#include <graphics/RenderObject.h>

RenderObject::RenderObject(TextureHandle handle)
    : m_handle(handle),
      m_screenBounds{ 0, 0, 0, 0},
      m_verts{
        0.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        
        0.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f
      },
      m_renderLayer(RenderLayer::Begin)
 {}


void RenderObject::setScreenVerts( float scX, float scY, float scW, float scH )
{
    m_screenBounds = { scX, scY, scW, scH };
    
    // Top Left
    m_verts[0] = scX;
    m_verts[1] = scY + scH;
    
    // Bottom Right
    m_verts[8] = scX + scW;
    m_verts[9] = scY;
    
    // Bottom Left
    m_verts[16] = scX;
    m_verts[17] = scY;
    
    // Top Left
    m_verts[24] = scX;
    m_verts[25] = scY + scH;
    
    // Top Right
    m_verts[32] = scX + scW;
    m_verts[33] = scY + scH;
    
    // Bottom Left
    m_verts[40] = scX + scW;
    m_verts[41] = scY;
}

void RenderObject::setTextureVerts( float texX, float texY, float texW, float texH )
{
    // Top Left
    m_verts[2] = texX;
    m_verts[3] = texY + texH;
    
    // Bottom Right
    m_verts[10] = texX + texW;
    m_verts[11] = texY;
    
    // Bottom Left
    m_verts[18] = texX;
    m_verts[19] = texY;
    
    // Top Left
    m_verts[26] = texX;
    m_verts[27] = texY + texH;
    
    // Top Right
    m_verts[34] = texX + texW;
    m_verts[35] = texY + texH;
    
    // Bottom Left
    m_verts[42] = texX + texW;
    m_verts[43] = texY;
}

void RenderObject::setColourVerts( float r, float g, float b, float a )
{
    for ( int i = 4; i < 48; i += 8)
    {
        m_verts[i+0] = r;
        m_verts[i+1] = g;
        m_verts[i+2] = b;
        m_verts[i+3] = a;
    }
}


void RenderObject::setScreenPosition( Vector2f pos )
{
    setScreenVerts(pos.x(), pos.y(), m_screenBounds.w(), m_screenBounds.h() );
}

void RenderObject::setScreenSize( Vector2f size )
{
    setScreenVerts(m_screenBounds.x(), m_screenBounds.y(), size.x(), size.y()  );
}

void RenderObject::setRenderLayer( RenderLayer layer )
{
    m_renderLayer = layer;
}

RectF const &RenderObject::getScreenBounds() const
{
    return m_screenBounds;
}



TextureHandle RenderObject::getHandle() const
{
    return m_handle;
}

std::array<GLfloat, 48> const &RenderObject::getVerts() const
{
    return m_verts;
}

bool RenderObject::operator<( RenderObject const &rhs ) const
{
    return m_renderLayer < rhs.m_renderLayer;
}






