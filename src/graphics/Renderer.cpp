#include <graphics/Renderer.h>

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <resource/ResourceManager.h>


// Render Buffer
// ----------------------


RenderBuffer::RenderBuffer()
    : vboIdx(0), vaoIdx(0), isHeld(false)
{
    glGenBuffers(1, &vboIdx);
    glGenVertexArrays(1, &vaoIdx);
    glGenBuffers(1, &eboIdx);
    
//    static GLuint EBO_INDICES[] = {0, 1, 3, 1, 2, 3};
//    static GLuint EBO_INDICES[] = {0, 1, 2, 3, 4, 5};
    static GLuint EBO_INDICES[] = {0, 1, 2, 0, 4, 1};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIdx);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(EBO_INDICES), EBO_INDICES, GL_STATIC_DRAW);
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
        m_buffers.push_back({});
    }
}


void Renderer::render()
{
    // Render most of the layers with the camera transform
    m_colourShader->setUniformMat4v( "model", m_cameraTransform );
    m_quadShader->setUniformMat4v( "model", m_cameraTransform );

    for (int i = 0; i < (int)RenderLayer::UI; i++)
    {
        renderBuffer(i);
    }
    
    // Switch to the identity transform for the UI layer
    m_quadShader->setUniformMat4v( "model", m_identity );
    m_colourShader->setUniformMat4v( "model", m_identity );
    
    renderBuffer((int)RenderLayer::UI);
}

void Renderer::addItem( RenderObject const &robj, RenderLayer layer )
{
    auto idx = (std::size_t) layer;
    
    if ( !m_buffers[idx].renderObjs.empty() && m_buffers[idx].renderObjs.back().canMerge(robj) )
    {
        m_buffers[idx].renderObjs.back().merge(robj);
    }
    else
    {
        m_buffers[idx].renderObjs.push_back( robj );
    }
}

void Renderer::setCameraOffset( Vector2f offset )
{
    m_cameraTransform = glm::translate( m_identity, glm::vec3(offset.x(), offset.y(), 0.0f) );
}

void Renderer::setWindowSize( Vector2f size )
{
    m_windowSize = size.convert<int>();
    glm::mat4 projection = glm::ortho(0.0f, size.x(), size.y(), 0.0f, -1.0f, 1.0f);
    
    m_quadShader->setUniformMat4v("projection", projection );
    m_textShader->setUniformMat4v("projection", projection );
    m_colourShader->setUniformMat4v("projection", projection );
}

void Renderer::holdBuffer(RenderLayer layer)
{
    m_buffers[(int)layer].isHeld = true;
}

void Renderer::releaseBuffer(RenderLayer layer)
{
    m_buffers[(int)layer].isHeld = false;
    m_buffers[(int)layer].renderObjs.clear();
}

void Renderer::renderBuffer( int idx )
{
    int currShader = -1;
    RectI currScissor = {0, 0, 0, 0};
    
    for ( auto obj : m_buffers[idx].renderObjs )
    {
        if ( obj.getShaderType() != currShader )
        {
            currShader = obj.getShaderType();
            glUseProgram( m_shaderHandles[currShader] );
        }
        if ( obj.getScissor() != currScissor )
        {
            currScissor = obj.getScissor();
            if (currScissor == RectI{0, 0, 0, 0})
            {
                // Disable scissor
                glDisable(GL_SCISSOR_TEST);
                
            }
            else
            {
                // Enable scissor
                glEnable(GL_SCISSOR_TEST);
                
                // We need to convert from top-left origin to bottom-left origin
                glScissor(
                    currScissor.x(),
                    m_windowSize.y() - currScissor.y() - currScissor.h(),
                    currScissor.w(),
                    currScissor.h()
                );
            }
        }
        
        // Bind texture if exists (not all shaders need one)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.getHandle());
        
        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[idx].vboIdx);
        glBufferData(GL_ARRAY_BUFFER, obj.getDataSize() * sizeof(GLfloat), obj.getData(), GL_STATIC_DRAW);
        
        glBindVertexArray(m_buffers[idx].vaoIdx);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[idx].eboIdx);
    
    
        // Texture + UV data
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Colour data
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Draw!
//        glDrawArrays(GL_TRIANGLES, 0, (obj.getDataSize() / RenderObject::FLOATS_PER_QUAD) * RenderObject::TRIANGLES_PER_QUAD );

        int dQuadCount = obj.getDataSize() / RenderObject::FLOATS_PER_QUAD;
        int dVertCount = dQuadCount * RenderObject::TRIANGLES_PER_QUAD;
        glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_INT, NULL);
//        glDrawElements(GL_TRIANGLES, (obj.getDataSize() / RenderObject::FLOATS_PER_QUAD) * RenderObject::TRIANGLES_PER_QUAD, GL_UNSIGNED_INT, NULL);
    
    }
    
    if ( !m_buffers[idx].isHeld )
    {
        m_buffers[idx].renderObjs.clear();
    }
}
