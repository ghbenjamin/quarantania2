#include <graphics/Renderer.h>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <resource/ResourceManager.h>


// Render Buffer
// ----------------------


RenderBuffer::RenderBuffer(std::vector<ShaderHandle> const& shaders )
    : m_vbo(0), m_vao(0), m_shaderHandles(shaders), m_isHeld(false)
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
    int currShader = -1;

    for ( auto obj : m_data )
    {
        if ( obj.getShaderType() != currShader )
        {
            currShader = obj.getShaderType();
            glUseProgram( m_shaderHandles[currShader] );
        }
    
        // Bind texture if exists (not all shaders need one)
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

    if ( !m_isHeld )
    {
        m_data.clear();
    }
}

void RenderBuffer::holdBuffer()
{
    m_isHeld = true;
}

void RenderBuffer::releaseBuffer()
{
    m_isHeld = false;
    m_data.clear();
}


// Renderer
//-----------------------


void Renderer::init( Vector2f windowSize )
{
    m_identity = glm::mat4(1.0f);
    m_cameraTransform = glm::mat4(1.0f);
    
    m_textShader = ResourceManager::get().getShaderProgram( "text_shader" ).getProgram();
    m_textShader->setUniformMat4v( "model", m_identity );
    
    m_colourShader = ResourceManager::get().getShaderProgram( "colour_only_shader" ).getProgram();
    m_colourShader->setUniformMat4v( "model", m_identity );
    
    m_quadShader = ResourceManager::get().getShaderProgram( "quad_shader" ).getProgram();
    
    
    setWindowSize(windowSize);
    
    m_shaderHandles = { m_quadShader->getHandle(), m_textShader->getHandle(), m_colourShader->getHandle() };
    
    for (int i = 0; i < RENDER_LAYER_COUNT; i++)
    {
        m_buffers.push_back({ m_shaderHandles });
    }
}


void Renderer::render()
{
    // Render most of the layers with the camera transform
    m_colourShader->setUniformMat4v( "model", m_cameraTransform );
    m_quadShader->setUniformMat4v( "model", m_cameraTransform );

    for (int i = 0; i < (int)RenderLayer::UI; i++)
    {
        m_buffers[i].render();
    }
    
    // Switch to the identity transform for the UI layer
    m_quadShader->setUniformMat4v( "model", m_identity );
    m_colourShader->setUniformMat4v( "model", m_identity );

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
    
    m_quadShader->setUniformMat4v("projection", projection );
    m_textShader->setUniformMat4v("projection", projection );
    m_colourShader->setUniformMat4v("projection", projection );
}

void Renderer::holdBuffer(RenderLayer layer)
{
    m_buffers[(int)layer].holdBuffer();
}

void Renderer::releaseBuffer(RenderLayer layer)
{
    m_buffers[(int)layer].releaseBuffer();
}
