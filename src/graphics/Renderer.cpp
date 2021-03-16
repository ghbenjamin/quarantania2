#include <graphics/Renderer.h>

// Render Buffer
// ----------------------


RenderBuffer::RenderBuffer()
    : m_vbo(0), m_vao(0)
{
    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);
}

void RenderBuffer::addItem( RenderObject const &robj )
{
    if ( !m_data.empty() && robj.getHandle() == m_data.back().getHandle() )
    {
        m_data.back().merge(robj);
    }
    else
    {
        m_data.push_back( robj );
    }
}

void RenderBuffer::render()
{
    for ( auto obj : m_data )
    {
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.getHandle());
        
        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, obj.getDataSize() * sizeof(GLfloat), obj.getData(), GL_STATIC_DRAW);
    
    
        glBindVertexArray(m_vao);
        
        // Texture + UV data
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Colour data
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Draw!
        glDrawArrays(GL_TRIANGLES, 0, (obj.getDataSize() / RenderObject::FLOATS_PER_QUAD) * RenderObject::TRIANGLES_PER_QUAD );
    }
}


// Renderer
//-----------------------

Renderer::Renderer()
{

}

void Renderer::init()
{
    for (int i = 0; i < RENDER_LAYER_COUNT; i++)
    {
        m_buffers.push_back({});
    }
}


void Renderer::render()
{
    for (int i = 0; i < RENDER_LAYER_COUNT; i++)
    {
        m_buffers[i].render();
    }
}

void Renderer::addItem( RenderObject const &robj, RenderLayer layer )
{
    auto idx = (std::size_t) layer;
    m_buffers[idx].addItem( robj );
}




