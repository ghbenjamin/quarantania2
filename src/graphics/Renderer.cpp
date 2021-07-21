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


Renderer::Renderer( Vector2f initWndSize )
{
    // No depth testing or face culling
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set the viewport size and projection
    glViewport(0, 0, (GLsizei)initWndSize.x(), (GLsizei)initWndSize.y());

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Uncomment me to turn on wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_identity = glm::mat4(1.0f);
    m_cameraTransform = glm::mat4(1.0f);

    m_textShader = ResourceManager::get().getShaderProgram( "text_shader" ).getProgram();
    m_textShader->setUniformMat4v( "model", m_identity );
    m_textShader->setUniformMat4v( "localTransform", m_identity );

    m_colourShader = ResourceManager::get().getShaderProgram( "colour_only_shader" ).getProgram();
    m_colourShader->setUniformMat4v( "model", m_identity );
    m_colourShader->setUniformMat4v( "localTransform", m_identity );

    m_quadShader = ResourceManager::get().getShaderProgram( "quad_shader" ).getProgram();
    m_quadShader->setUniformMat4v( "localTransform", m_identity );

    m_noProjShader = ResourceManager::get().getShaderProgram( "no_projection" ).getProgram();
    
    m_sceneFadeout = ResourceManager::get().getShaderProgram( "scene_fadeout" ).getProgram();
    m_sceneFadeout->setUniformFloat( "fadeFrac", 1.0f );

    setWindowSize(initWndSize);

    m_allShaders = {
        m_quadShader,
        m_textShader,
        m_colourShader,
        m_noProjShader,
        m_sceneFadeout
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


    Assert( glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create a fullscreen quad render object using the scene framebuffer object
    RenderObject fullscreenQuad = RenderObject( m_fbTexture, ShaderType::SceneFadeout );
    fullscreenQuad.setScreenVerts(0, -1.0f, -1.0f, 2.f, 2.f);
    fullscreenQuad.setTextureVerts(0, 0.f, 0.f, 1.f, 1.f);
    m_pprocessBuffer.renderObjs.push_back(fullscreenQuad);
    m_pprocessBuffer.isHeld = true;
}

Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &m_fbFrameBuffer);
}


void Renderer::render()
{
    // Render our scene to a framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbFrameBuffer);

    // Clear the contents of the framebuffer
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render most of the layers with the camera transform
    m_colourShader->setUniformMat4v( "model", m_cameraTransform );
    m_quadShader->setUniformMat4v( "model", m_cameraTransform );

    for (int i = 0; i < (int)RenderLayer::UI; i++)
    {
        renderBuffer( &m_buffers[i] );
    }


    // Render our UI

    // Switch to the identity transform for the UI layer
    m_quadShader->setUniformMat4v( "model", m_identity );
    m_colourShader->setUniformMat4v( "model", m_identity );

    renderBuffer( &m_buffers[(int)RenderLayer::UI] );

    // Render the framebuffer to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear the screen behind the framebuffer
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderBuffer( &m_pprocessBuffer );
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
    glViewport(0, 0, (GLsizei)size.x(), (GLsizei)size.y());

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
    std::shared_ptr<ShaderProgram> currentShader;
    int currShader = -1;
    RectI currScissor = {0, 0, 0, 0};
    glm::mat4 currentTransform = glm::mat4(1.0f);

    for ( auto obj : buf->renderObjs )
    {
        if ( obj.getShaderType() != currShader )
        {
            currShader = obj.getShaderType();
            currentShader = m_allShaders[currShader];
            currentShader->setUniformMat4v("localTransform", m_identity);
            currentShader->useProgram();
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
        if ( obj.getTransform() != currentTransform )
        {
            currentTransform = obj.getTransform();
            currentShader->setUniformMat4v("localTransform", currentTransform);
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

    glDisable(GL_SCISSOR_TEST);

    if ( !buf->isHeld )
    {
        buf->renderObjs.clear();
    }
}

void Renderer::update(std::uint32_t ticks)
{
    if ( m_fadeTimer.has_value() )
    {
        m_fadeTimer->advance(ticks);
        
        m_sceneFadeout->setUniformFloat( "fadeFrac", m_fadeTimer->current() );
        
        if ( m_fadeTimer->isFinished() )
        {
            if (m_isFadingOut)
            {
                // We've finished fading out - leave the shader as is for now
            }
            else
            {
                // We've finished fading in - switch back to the regular shader
                m_pprocessBuffer.renderObjs.front().setShader(ShaderType::NoProjection);
            }
        }
    }
}

void Renderer::startFadeout( float time )
{
    m_fadeTimer = TimedLinearInterpolator<float>(1.0f, 0.0f, time);
    m_isFadingOut = true;
}

void Renderer::startFadein( float time )
{
    m_pprocessBuffer.renderObjs.front().setShader(ShaderType::SceneFadeout);
    m_fadeTimer = TimedLinearInterpolator<float>(0.0f, 1.0f, time);
    m_isFadingOut = false;
}

bool Renderer::isMidTransition() const
{
   return m_fadeTimer.has_value() && !m_fadeTimer->isFinished();
}


