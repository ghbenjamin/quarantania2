#include <graphics/RenderObject.h>

RenderObject::RenderObject(TextureHandle handle, ShaderType shader)
    : m_handle(handle),
      m_shader(shader),
      m_scissor(0, 0, 0, 0),
      m_data(32, 0.0f),
      m_quadCount(1)
{
    m_data = {
        0.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f, // Top Left
        1.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom Right
        0.0f, 0.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f, // Bottom Left
        1.0f, 1.0f, 0, 0, 1.0f, 1.0f, 1.0f, 1.0f, // Top Right
    };

    m_indexes = {
        0, 1, 2,
        0, 3, 1,
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

    // Top Right
    m_data[idx*FLOATS_PER_QUAD+26] = texX + texW;
    m_data[idx*FLOATS_PER_QUAD+27] = texY + texH;
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

    // Top Right
    m_data[idx*FLOATS_PER_QUAD+24] = scX + scW;
    m_data[idx*FLOATS_PER_QUAD+25] = scY + scH;
}

void
RenderObject::setScreenVerts( int idx, Vector2f topLeft, Vector2f bottomRight, Vector2f bottomLeft, Vector2f topRight )
{
    // Top Left
    m_data[idx*FLOATS_PER_QUAD+0] = topLeft.x();
    m_data[idx*FLOATS_PER_QUAD+1] = topLeft.y();
    
    // Bottom Right
    m_data[idx*FLOATS_PER_QUAD+8] = bottomRight.x();
    m_data[idx*FLOATS_PER_QUAD+9] = bottomRight.y();
    
    // Bottom Left
    m_data[idx*FLOATS_PER_QUAD+16] = bottomLeft.x();
    m_data[idx*FLOATS_PER_QUAD+17] = bottomLeft.y();
    
    // Top Right
    m_data[idx*FLOATS_PER_QUAD+24] = topRight.x();
    m_data[idx*FLOATS_PER_QUAD+25] = topRight.y();
}

void RenderObject::setColourVerts( int idx, float r, float g, float b, float a )
{
    for ( int i = 4; i < FLOATS_PER_QUAD; i += VERTEX_STRIDE_BYTES)
    {
        m_data[idx*FLOATS_PER_QUAD+i+0] = r;
        m_data[idx*FLOATS_PER_QUAD+i+1] = g;
        m_data[idx*FLOATS_PER_QUAD+i+2] = b;
        m_data[idx*FLOATS_PER_QUAD+i+3] = a;
    }
}

void RenderObject::setAlphaVerts( int idx, float a )
{
    for ( int i = 4; i < FLOATS_PER_QUAD; i += VERTEX_STRIDE_BYTES)
    {
        m_data[idx*FLOATS_PER_QUAD+i+3] = a;
    }
}


void RenderObject::merge( RenderObject other )
{
    m_data.insert( m_data.end(), other.m_data.begin(), other.m_data.end() );

    for (int i = m_quadCount; i < m_quadCount + other.m_quadCount; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            m_indexes.push_back( m_indexes[j] + (4 * i) );
        }
    }

    m_quadCount += other.m_quadCount;
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

    for (int j = 0; j < 6; j++)
    {
        m_indexes.push_back( m_indexes[j] + (4 * m_quadCount) );
    }

    m_quadCount++;
}

int RenderObject::getShaderType() const
{
    return (int) m_shader;
}

bool RenderObject::canMerge( RenderObject const &other ) const
{
    return m_handle == other.m_handle && m_shader == other.m_shader && m_scissor == other.m_scissor;
}

void RenderObject::setScissor( RectI bounds )
{
    m_scissor = bounds;
}

void RenderObject::removeScissor()
{
    m_scissor = {0, 0, 0, 0};
}

RectI const &RenderObject::getScissor() const
{
    return m_scissor;
}

GLuint *RenderObject::getIndexes()
{
    return m_indexes.data();
}

int RenderObject::getIndexSize() const
{
    return m_indexes.size() * sizeof(GLuint);
}

int RenderObject::getQuadCount() const
{
    return m_quadCount;
}

void RenderObject::setShader( ShaderType type )
{
    m_shader = type;
}








