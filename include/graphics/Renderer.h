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
    Renderer() = default;
    ~Renderer();
    
    // Delayed initialization until OpenGL context has been created
    void init( Vector2f windowSize );
    
    // Draw the contents of our buffer, then empty the buffers
    void render();
    
    // Add an item to the render queue.
    void addItem( RenderObject const& robj, RenderLayer layer );
    
    // Set the camera offset vector used when drawing objects in world space
    void setCameraOffset( Vector2f offset );
    
    void setWindowSize( Vector2f size );

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
    GLuint m_fbRenderBuffer;
    RenderObject m_fbQuad;
    RenderBuffer m_pprocessBuffer;
};