#pragma once

#include <glm/glm.hpp>

#include <graphics/RenderObject.h>

class ShaderProgram;

struct RenderBuffer
{
    RenderBuffer();
    ~RenderBuffer() = default;
    
    std::vector<RenderObject> renderObjs;
    GLuint vboIdx;
    GLuint vaoIdx;
    GLuint eboIdx;
    bool isHeld;
};


class Renderer
{
public:
    Renderer( Vector2f initWndSize );
    ~Renderer();


    // Draw the contents of our buffer, then empty the buffers
    void render();

    void update( std::uint32_t ticks );
    
    // Add an item to the render queue.
    void addItem( RenderObject const& robj, RenderLayer layer );
    
    // Set the camera offset vector used when drawing objects in world space
    void setCameraOffset( Vector2f offset );

    // Update internal state that depends on the size of the window 
    void setWindowSize( Vector2f size );

    // Hold & release the buffer - while held, the buffer will stop clearing at the end of each tick,
    // and will instead just rerender what it has buffered.
    void holdBuffer( RenderLayer layer );
    void releaseBuffer( RenderLayer layer );

private:

    void renderBuffer( RenderBuffer* buf );
    

    // Render buffers, indexed by the RenderLayer enum
    std::vector<RenderBuffer> m_buffers;
    
    // Our shader programs.
    std::shared_ptr<ShaderProgram> m_quadShader;
    std::shared_ptr<ShaderProgram> m_textShader;
    std::shared_ptr<ShaderProgram> m_colourShader;
    std::shared_ptr<ShaderProgram> m_noProjShader;

    std::vector<ShaderHandle> m_shaderHandles;
    
    // Transformation matrices for world and screen space render objects
    glm::mat4 m_cameraTransform;
    glm::mat4 m_identity;
    
    Vector2i m_windowSize;

    GLuint m_fbFrameBuffer;
    GLuint m_fbTexture;
    RenderObject m_fbQuad;
    RenderBuffer m_pprocessBuffer;
};