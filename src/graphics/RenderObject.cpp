#include <graphics/RenderObject.h>

RenderObject::RenderObject(TextureHandle handle)
    : m_handle(handle),
      m_screenBounds{ 0, 0, 0, 0},
      m_verts{
        0.0f, 1.0f, 0, 0,
        1.0f, 0.0f, 0, 0,
        0.0f, 0.0f, 0, 0,
        
        0.0f, 1.0f, 0, 0,
        1.0f, 1.0f, 0, 0,
        1.0f, 0.0f, 0, 0
      },
      m_renderLayer(RenderLayer::Begin)
 {}


void RenderObject::setTextureVerts( float texX, float texY, float texW, float texH )
{
    // Top Left
    m_verts[2] = texX;
    m_verts[3] = texY + texH;
    
    // Bottom Right
    m_verts[6] = texX + texW;
    m_verts[7] = texY;
    
    // Bottom Left
    m_verts[10] = texX;
    m_verts[11] = texY;
    
    // Top Left
    m_verts[14] = texX;
    m_verts[15] = texY + texH;
    
    // Top Right
    m_verts[18] = texX + texW;
    m_verts[19] = texY + texH;
    
    // Bottom Left
    m_verts[22] = texX + texW;
    m_verts[23] = texY;
}

void RenderObject::setScreenVerts( float scX, float scY, float scW, float scH )
{
    m_screenBounds = { scX, scY, scW, scH };
    
    // Top Left
    m_verts[0] = scX;
    m_verts[1] = scY + scH;

    // Bottom Right
    m_verts[4] = scX + scW;
    m_verts[5] = scY;

    // Bottom Left
    m_verts[8] = scX;
    m_verts[9] = scY;

    // Top Left
    m_verts[12] = scX;
    m_verts[13] = scY + scH;

    // Top Right
    m_verts[16] = scX + scW;
    m_verts[17] = scY + scH;

    // Bottom Left
    m_verts[20] = scX + scW;
    m_verts[21] = scY;
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

std::array<GLfloat, 24> const &RenderObject::getVerts() const
{
    return m_verts;
}

bool RenderObject::operator<( RenderObject const &rhs ) const
{
    return m_renderLayer < rhs.m_renderLayer;
}




