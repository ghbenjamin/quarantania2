#include <graphics/Renderer.h>

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <resource/ResourceManager.h>


// Render Buffer
// ----------------------


RenderBuffer::RenderBuffer()
    : vboIdx(0), vaoIdx(0), eboIdx(0), isHeld(false)
{
    glGenVertexArrays(1, &vaoIdx);
    glGenBuffers(1, &vboIdx);
    glGenBuffers(1, &eboIdx);
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
    m_noProjShader = ResourceManager::get().getShaderProgram( "no_projection" ).getProgram();

    
    setWindowSize(windowSize);
    
    m_shaderHandles = {
            m_quadShader->getHandle(),
            m_textShader->getHandle(),
            m_colourShader->getHandle(),
            m_noProjShader->getHandle()
    };
    
    for (int i = 0; i < RENDER_LAYER_COUNT; i++)
    {
        m_buffers.push_back({});
    }


    // Create scene framebuffer
    glGenFramebuffers(1, &m_fbFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbFrameBuffer);

    // Generate a texture for the scene framebuffer
    glGenTextures(1, &m_fbTexture);
    glBindTexture(GL_TEXTURE_2D, m_fbTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_windowSize.x(), m_windowSize.y(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbTexture, 0);

    // Create a renderbuffer for the scene framebuffer
    glGenRenderbuffers(1, &m_fbRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_windowSize.x(), m_windowSize.y());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbRenderBuffer);

    Assert( glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create a fullscreen quad render object using the scene framebuffer object
    RenderObject fullscreenQuad = RenderObject( m_fbTexture, ShaderType::NoProjection );
    fullscreenQuad.setScreenVerts(0, -1.0f, -1.0f, 2.f, 2.f);
    fullscreenQuad.setTextureVerts(0, 0.f, 0.f, 1.f, 1.f);
    m_pprocessBuffer.renderObjs.push_back(fullscreenQuad);
    m_pprocessBuffer.isHeld = true;
}


void Renderer::render()
{
    // Render our scene to a framebuffer

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbFrameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render most of the layers with the camera transform
    m_colourShader->setUniformMat4v( "model", m_cameraTransform );
    m_quadShader->setUniformMat4v( "model", m_cameraTransform );

    for (int i = 0; i < (int)RenderLayer::UI; i++)
    {
        renderBuffer( &m_buffers[i] );
    }


    // Render the framebuffer to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    renderBuffer( &m_pprocessBuffer );


    // Render our UI

    // Switch to the identity transform for the UI layer
    m_quadShader->setUniformMat4v( "model", m_identity );
    m_colourShader->setUniformMat4v( "model", m_identity );

    renderBuffer( &m_buffers[(int)RenderLayer::UI] );
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

void Renderer::renderBuffer( RenderBuffer* buf )
{
    int currShader = -1;
    RectI currScissor = {0, 0, 0, 0};

    for ( auto obj : buf->renderObjs )
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

        // Bind VAO
        glBindVertexArray(buf->vaoIdx);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, buf->vboIdx);
        glBufferData(GL_ARRAY_BUFFER, obj.getDataSize() * sizeof(GLfloat), obj.getData(), GL_STATIC_DRAW);

        // Bind EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->eboIdx);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getIndexSize(), obj.getIndexes(), GL_STATIC_DRAW);

        // Texture + UV data
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Colour data
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Draw!
        glDrawElements(GL_TRIANGLES, obj.getQuadCount() * RenderObject::INDICES_PER_QUAD, GL_UNSIGNED_INT, 0);

    }
    
    if ( !buf->isHeld )
    {
        buf->renderObjs.clear();
    }
}

Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &m_fbFrameBuffer);
}
