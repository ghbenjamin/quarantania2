#include <graphics/RenderObject.h>

RenderObject::RenderObject(TextureHandle handle, ShaderType shader)
    : m_handle(handle),
      m_shader(shader),
      m_data(48, 0.0f)
{
    m_data = {
        0.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        
        0.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f
    };
}

RenderObject::RenderObject( TextureHandle handle )
 : RenderObject(handle, ShaderType::QuadShader) {}

RenderObject::RenderObject()
 : RenderObject(0) {}

TextureHandle RenderObject::getHandle() const
{
    return m_handle;
}

void RenderObject::setTextureVerts( int idx, float texX, float texY, float texW, float texH )
{
    // Top Left
    m_data[idx*FLOATS_PER_QUAD+2] = texX;
    m_data[idx*FLOATS_PER_QUAD+3] = texY + texH;
    
    // Bottom Right
    m_data[idx*FLOATS_PER_QUAD+10] = texX + texW;
    m_data[idx*FLOATS_PER_QUAD+11] = texY;
    
    // Bottom Left
    m_data[idx*FLOATS_PER_QUAD+18] = texX;
    m_data[idx*FLOATS_PER_QUAD+19] = texY;
    
    // Top Left
    m_data[idx*FLOATS_PER_QUAD+26] = texX;
    m_data[idx*FLOATS_PER_QUAD+27] = texY + texH;
    
    // Top Right
    m_data[idx*FLOATS_PER_QUAD+34] = texX + texW;
    m_data[idx*FLOATS_PER_QUAD+35] = texY + texH;
    
    // Bottom Left
    m_data[idx*FLOATS_PER_QUAD+42] = texX + texW;
    m_data[idx*FLOATS_PER_QUAD+43] = texY;
}

void RenderObject::setScreenVerts( int idx, float scX, float scY, float scW, float scH )
{
    // Top Left
    m_data[idx*FLOATS_PER_QUAD+0] = scX;
    m_data[idx*FLOATS_PER_QUAD+1] = scY + scH;
    
    // Bottom Right
    m_data[idx*FLOATS_PER_QUAD+8] = scX + scW;
    m_data[idx*FLOATS_PER_QUAD+9] = scY;
    
    // Bottom Left
    m_data[idx*FLOATS_PER_QUAD+16] = scX;
    m_data[idx*FLOATS_PER_QUAD+17] = scY;
    
    // Top Left
    m_data[idx*FLOATS_PER_QUAD+24] = scX;
    m_data[idx*FLOATS_PER_QUAD+25] = scY + scH;
    
    // Top Right
    m_data[idx*FLOATS_PER_QUAD+32] = scX + scW;
    m_data[idx*FLOATS_PER_QUAD+33] = scY + scH;
    
    // Bottom Left
    m_data[idx*FLOATS_PER_QUAD+40] = scX + scW;
    m_data[idx*FLOATS_PER_QUAD+41] = scY;
}

void RenderObject::setColourVerts( int idx, float r, float g, float b, float a )
{
    for ( int i = 4; i < FLOATS_PER_QUAD; i += 8)
    {
        m_data[idx*FLOATS_PER_QUAD+i+0] = r;
        m_data[idx*FLOATS_PER_QUAD+i+1] = g;
        m_data[idx*FLOATS_PER_QUAD+i+2] = b;
        m_data[idx*FLOATS_PER_QUAD+i+3] = a;
    }
}

void RenderObject::merge( RenderObject other )
{
    m_data.reserve( m_data.size() + other.m_data.size() );
    m_data.insert( m_data.end(), other.m_data.begin(), other.m_data.end() );
}

GLfloat *RenderObject::getData()
{
    return m_data.data();
}

int RenderObject::getDataSize()
{
    return m_data.size();
}

void RenderObject::addQuad( RectF screenOffsets, RectF uvBounds )
{
    int idx = (m_data.size() / FLOATS_PER_QUAD);
    m_data.resize( m_data.size() + FLOATS_PER_QUAD, 0.0f );
    
    setScreenVerts(idx, screenOffsets.x(), screenOffsets.y(), screenOffsets.w(), screenOffsets.h() );
    setTextureVerts(idx, uvBounds.x(), uvBounds.y(), uvBounds.w(), uvBounds.h() );
    setColourVerts(idx, 0.0f, 0.0f, 0.0f, 1.0f);
}

int RenderObject::getShaderType() const
{
    return (int) m_shader;
}





