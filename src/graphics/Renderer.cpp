#include <graphics/Renderer.h>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/Shader.h>


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
    
    m_data.clear();
}


// Renderer
//-----------------------


void Renderer::init( Vector2f windowSize )
{
    for (int i = 0; i < RENDER_LAYER_COUNT; i++)
    {
        m_buffers.push_back({});
    }
    
    m_identity = glm::mat4(1.0f);
    m_cameraTransform = glm::mat4(1.0f);
    
    m_shaderProgram = std::make_shared<ShaderProgram>( "simple_screenspace", "simple_sampler" );
    m_shaderProgram->useProgram();
    
    m_modelLoc = m_shaderProgram->getUniformLocation( "model" );
    m_projectionLoc = m_shaderProgram->getUniformLocation( "projection" );
    
    setWindowSize(windowSize);
}


void Renderer::render()
{
    // Render most of the layers with the camera transform
    m_shaderProgram->setUniformMat4v( m_modelLoc, m_cameraTransform );
    for (int i = 0; i < (int)RenderLayer::UI; i++)
    {
        m_buffers[i].render();
    }
    
    // Switch to the identity transform for the UI layer
    m_shaderProgram->setUniformMat4v( m_modelLoc, m_identity );
    m_buffers[(int)RenderLayer::UI].render();
}

void Renderer::addItem( RenderObject const &robj, RenderLayer layer )
{
    auto idx = (std::size_t) layer;
    m_buffers[idx].addItem( robj );
}

void Renderer::setCameraOffset( Vector2f offset )
{
    m_cameraTransform = glm::translate( m_identity, glm::vec3(offset.x(), offset.y(), 0.0f) );
}

void Renderer::setWindowSize( Vector2f size )
{
    glm::mat4 projection = glm::ortho(0.0f, size.x(), size.y(), 0.0f, -1.0f, 1.0f);
    m_shaderProgram->setUniformMat4v( m_projectionLoc, projection );
}
